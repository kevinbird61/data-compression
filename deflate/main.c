#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "zlib/zlib.h"

typedef struct _BIN_FILE {
    char *byte;
    uint32_t filelen;
} BIN_FILE;

uint32_t fsize (FILE *in)
{
    uint32_t length;
    fseek(in, 0, SEEK_END);
    length = ftell(in);
    rewind(in);
    return length;
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

    // read file
    FILE *fileptr;
    char *buf;
    uint32_t filelen;
    fileptr = fopen(argv[1], "rb");
    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);

    buf = (char *)malloc((filelen+1)*sizeof(char));
    fread(buf, filelen, 1, fileptr);
    fclose(fileptr);


    printf("Original size: %u bytes, %u KB\n", filelen, filelen/1000);
    printf("Original strlen: %lu\n", strlen(buf));
    // printf("%s\n", buf);

    // declare for temp storage
    char *compress_text, *decompress_text;

    compress_text = (char *)malloc((filelen+1)*sizeof(char));
    decompress_text = (char *)malloc((filelen+1)*sizeof(char));

    //=================== compress - deflate ==========================
    // zlib struct 
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup file->byte as input, and compress_text as output
    defstream.avail_in = filelen+1;
    defstream.next_in = (Bytef *)buf;
    defstream.avail_out = filelen+1;
    defstream.next_out = (Bytef *)compress_text;

    // actual compression work 
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    // This is one way of getting the size of the output
    printf("Compressed size is: %lu bytes\n", defstream.total_out);
    printf("\n----------\n\n");

    /* TODO: output this compressed file */


    //=================== compress - inflate ==========================
    z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.avail_in = (uInt)((char*) defstream.next_out - compress_text);
    infstream.next_in = (Bytef *) compress_text;
    infstream.avail_out = filelen+1;
    infstream.next_out = (Bytef *) decompress_text; 

    // actual decompression work
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);
    // This is one way of getting the size of the output
    printf("Uncompressed size is: %lu bytes\n", infstream.total_out);
    printf("\n----------\n\n");

    // make sure uncompressed is exactly equal to original
    assert(strcmp(buf, decompress_text)==0);

    return 0;
}