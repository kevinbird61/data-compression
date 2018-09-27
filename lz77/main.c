/**
 * Inspire from github source code:  https://github.com/andyherbert/lz1/blob/master/lz.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct _BIN_FILE {
    uint8_t *byte;
    uint32_t filelen;
} BIN_FILE;

// Read file into byte string
BIN_FILE *read_file_bin(char *filename){
    FILE *fileptr;
    uint8_t *buf;
    uint32_t filelen;

    fileptr = fopen(filename, "rb");

    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);

    buf = (uint8_t *)malloc((filelen+1)*sizeof(char));
    fread(buf, filelen, 1, fileptr);
    fclose(fileptr);

    // PACK
    BIN_FILE *obj = (BIN_FILE *)malloc(sizeof(BIN_FILE));
    obj->byte = buf;
    obj->filelen = filelen;

    return obj;
}

uint32_t fsize (FILE *in)
{
    uint32_t length;
    fseek(in, 0, SEEK_END);
    length = ftell(in);
    rewind(in);
    return length;
}

uint32_t lz77_compress_size(uint8_t *uncompressed_text, uint32_t uncompressed_size, uint8_t *compressed_text, uint8_t pointer_length_width){
    uint16_t pointer_pos, temp_pointer_pos, output_pointer, pointer_length, temp_pointer_length;
    uint32_t compressed_pointer, output_size, coding_pos, output_lookahead_ref, look_behind, look_ahead;
    uint16_t pointer_pos_max, pointer_length_max;
    pointer_pos_max = pow(2, 16 - pointer_length_width);
    pointer_length_max = pow(2, pointer_length_width);

    *((uint32_t *) compressed_text) = uncompressed_size;
    *(compressed_text + 4) = pointer_length_width;
    compressed_pointer = output_size = 5;

    for(coding_pos=0; coding_pos < uncompressed_size; coding_pos++){
        pointer_pos = 0;
        pointer_length = 0;
        for(temp_pointer_pos=1; (temp_pointer_pos < pointer_pos_max) && (temp_pointer_pos <= coding_pos); temp_pointer_pos++){
            look_behind = coding_pos - temp_pointer_pos;
            look_ahead = coding_pos;
            for(temp_pointer_length=0; uncompressed_text[look_ahead++] == uncompressed_text[look_behind++]; ++temp_pointer_length){
                if(temp_pointer_length == pointer_length_max)
                    break;
            }
            if(temp_pointer_length > pointer_length){
                pointer_pos = temp_pointer_pos;
                pointer_length = temp_pointer_length;
                if(pointer_length == pointer_length_max)
                    break;
            }
        }

        coding_pos += pointer_length;
        if((coding_pos == uncompressed_size) && pointer_length){
            output_pointer = (pointer_length == 1) ? 0 : ((pointer_pos << pointer_length_width) | (pointer_length - 2));
            output_lookahead_ref = coding_pos - 1;
        } else {
            output_pointer = (pointer_pos << pointer_length_width) | (pointer_length ? (pointer_length - 1) : 0);
            output_lookahead_ref = coding_pos;
        }

        *((uint16_t *) (compressed_text + compressed_pointer)) = output_pointer;
        compressed_pointer += 2;
        *(compressed_text + compressed_pointer++) = *(uncompressed_text + output_lookahead_ref);
        output_size += 3;
    }

    return output_size;
}

uint32_t lz77_decompress_size(uint8_t *compressed_text, uint8_t *uncompressed_text){
    uint8_t pointer_length_width;
    uint16_t input_pointer, pointer_length, pointer_pos, pointer_length_mask;
    uint32_t compressed_pointer, coding_pos, pointer_offset, uncompressed_size;

    uncompressed_size = *((uint32_t *) compressed_text);
    pointer_length_width = *(compressed_text + 4);
    compressed_pointer = 5;

    pointer_length_mask = pow(2, pointer_length_width) - 1;

    for(coding_pos = 0; coding_pos < uncompressed_size; ++coding_pos)
    {
        input_pointer = *((uint16_t *) (compressed_text + compressed_pointer));
        compressed_pointer += 2;
        pointer_pos = input_pointer >> pointer_length_width;
        pointer_length = pointer_pos ? ((input_pointer & pointer_length_mask) + 1) : 0;
        if(pointer_pos)
            for(pointer_offset = coding_pos - pointer_pos; pointer_length > 0; --pointer_length)
                uncompressed_text[coding_pos++] = uncompressed_text[pointer_offset++];
        *(uncompressed_text + coding_pos) = *(compressed_text + compressed_pointer++);
    }

    return coding_pos;
}

// lz77 compressing process
BIN_FILE *lz77_compress(const char *filename_in, const char *filename_out, size_t malloc_size, uint8_t pointer_length_width){
    FILE *in, *out;
    uint8_t *uncompr_text, *compr_text;
    uint32_t uncompr_size, compr_size;

    in = fopen(filename_in, "rb");
    if(in == NULL){
        printf("Open file error in function - lz77_compress.\n");
        exit(EXIT_FAILURE);
    }
    uncompr_size = fsize(in);
    uncompr_text = (uint8_t *)malloc((uncompr_size+1)*sizeof(uint8_t));
    if((uncompr_size != fread(uncompr_text, sizeof(uint8_t), uncompr_size, in))){
        printf("Miss matching size of uncompression file in function - lz77_compress.\n");
        exit(EXIT_FAILURE);
    }
    fclose(in);

    compr_text = (uint8_t *)malloc((malloc_size+1)*sizeof(uint8_t));
    // compute compress size 
    compr_size = lz77_compress_size(uncompr_text, uncompr_size, compr_text, pointer_length_width);

    // return with compress obj
    BIN_FILE *obj = (BIN_FILE *)malloc(sizeof(BIN_FILE));
    obj->byte = compr_text;
    obj->filelen = compr_size;

    free(compr_text);
    free(uncompr_text);

    return obj;
}

// lz77 decompressing process
BIN_FILE *lz77_decompress(const char *compressed_filename_in, const char *filename_out){
    FILE *in, out;
    uint8_t *uncompr_text, *compr_text;
    uint32_t compr_size, uncompr_size, result;

    in = fopen(compressed_filename_in, "rb");
    if(in == NULL){
        printf("Open file error in function - lz77_decompress.\n");
        exit(EXIT_FAILURE);
    }

    compr_size = fsize(in);
    compr_text = (uint8_t *)malloc((compr_size+1)*sizeof(uint8_t));

    if(fread(compr_text, sizeof(uint8_t), compr_size, in) != compr_size){
        printf("Read file error in function - lz77_decompress.\n");
        exit(EXIT_FAILURE);
    }
    fclose(in);

    uncompr_size = *((uint32_t *) compr_text);
    uncompr_text = (uint8_t *)malloc((uncompr_size+1)*sizeof(uint8_t));

    result = lz77_decompress_size(compr_text, uncompr_text);

    if(result != uncompr_size){
        printf("LZ77 decompress error: miss matching size. (result: %u, uncompressed size: %u)\n", result, uncompr_size);
        // exit(EXIT_FAILURE);
    }

    // return with decompress obj
    BIN_FILE *obj = (BIN_FILE *)malloc(sizeof(BIN_FILE));
    obj->byte = uncompr_text;
    obj->filelen = uncompr_size;

    free(compr_text);
    free(uncompr_text);

    return obj;
}

int main(int argc, char *argv[]){
    /* argv usage:
        [1]: input file 
        [2]: output file
    */

    if(argc < 3){
        printf("Need to assign input_file, output_file: \n./<executable> <input_filename.extension> <output_filename.extension>\n");
        exit(EXIT_FAILURE);
    }

    BIN_FILE *bin = read_file_bin(argv[1]);
    printf("Original size: %u bytes, %u KB\n", bin->filelen, bin->filelen/1000);

    // compression/decompression ,with changing pow factor
    for(uint8_t i=1; i<16; i++){
        // compressing process 
        BIN_FILE *test = lz77_compress(argv[1], "lz.z77", 10000000, i);
        // print out result
        printf("Compressed size(%i): %u. Compressed Ratio: %lf %%\n", i, test->filelen, 
            bin->filelen > test->filelen ? (bin->filelen - test->filelen)*100/(float)bin->filelen : 
                ((double)bin->filelen - test->filelen)*100/(float)bin->filelen );

        FILE *o = fopen("lz.z77", "wb");
        fwrite(test->byte, test->filelen, 1, o);
        fclose(o);
        free(test);
    }

    BIN_FILE *decompr = lz77_decompress("lz.z77", argv[2]);
    if(decompr->filelen == bin->filelen){
        printf("Success!\n");
        FILE *o = fopen(argv[2], "wb");
        fwrite(decompr->byte, decompr->filelen, 1, o);
        fclose(o);
    }
    else{
        printf("Failure! Size miss matching!\n");
    }
    

    // write test
    /*
    FILE *o = fopen(argv[2], "wb");
    fwrite(bin->byte, bin->filelen, 1, o);
    fclose(o);
    */

    return 0;
}