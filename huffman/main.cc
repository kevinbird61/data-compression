#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <vector>

/*
    Implement the huffman code tree
*/

// Data structure
struct huffman_tree{
    // huffman code
    std::string huffman_code();
    int size;
    char orig;
    // truncate 
    struct huffman_tree *left;
    struct huffman_tree *right;
    struct huffman_tree *parent;
};

// create vector for huffman usage
std::vector<huffman_tree *> tree;

// compressed var
int original_bits=0;
int compressed_bits=0;

// for sorting
bool comp(huffman_tree *lvalue,huffman_tree *rvalue){
    return (lvalue->size > rvalue->size);
}

// traversal
void traversal(huffman_tree *root,std::string str){
    if(root->left!=NULL)
        traversal(root->left,str+std::string("0"));
    if(root->right!=NULL)
        traversal(root->right,str+std::string("1"));
    if(root->left==NULL && root->right==NULL){
        printf("Character: %c, with size: %d, have huffman code: %s\n",root->orig,root->size,str.c_str());
        // Compute compressed bits
        /**
         * Take original character need 8 bits for storage
         * After compression => str.c_str()
         */
        original_bits+=8*(root->size);
        compressed_bits+=(str.length())*(root->size);
    }
    
    return;
}

// Main 
int main(int argc, char *argv[]){
    /**
     * argv[1] = uncompressed source code
     */

    int counter[256] = {0};
    /*for(int i=0;i<256;i++)
        counter[i]=0;*/

    FILE *fileptr;
    uint8_t *buf;
    uint32_t filelen;
    fileptr = fopen(argv[1], "rb");
    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);

    buf = (uint8_t *)malloc((filelen+1)*sizeof(char));
    fread(buf, filelen, 1, fileptr);
    fclose(fileptr);

    for(int i=0; i<filelen; i++){
        counter[(int)buf[i]]++;
    }
    

    for(int i=33;i<127;i++){
        huffman_tree *newnode=(huffman_tree*)malloc(sizeof(huffman_tree));
        newnode->size=counter[i];
        newnode->orig=(char)i;
        newnode->left=NULL;
        newnode->right=NULL;
        newnode->parent=NULL;
        // push into
        tree.push_back(newnode);
    }
    for(int i=128;i<256;i++){
        huffman_tree *newnode=(huffman_tree*)malloc(sizeof(huffman_tree));
        newnode->size=counter[i];
        newnode->orig=(char)i;
        newnode->left=NULL;
        newnode->right=NULL;
        newnode->parent=NULL;
        // push into
        tree.push_back(newnode);
    }

    // loop
    while(tree.size()!=1){
        // Step 1: sorting
        std::sort(tree.begin(),tree.end(),comp);
        // Step 2: pop out the last 2 element
        huffman_tree *node1,*node2,*result=(huffman_tree*)malloc(sizeof(huffman_tree));
        node1 = tree.back();
        tree.pop_back();
        node2 = tree.back();
        tree.pop_back();
        // create new node, as these 2 elements' parent node
        result->size=(node1->size)+(node2->size);
        // truncate these 3 node
        if(node1->size > node2->size){
            result->left = node2;
            result->right = node1;
        }
        else{
            result->left = node1;
            result->right = node2;
        }
        node1->parent=result;
        node2->parent=result;
        // push this new node back to tree
        tree.push_back(result);
    }

    // now the only element in "tree" is the root of huffman tree
    printf("Root node is here, with size: %d\n",tree.at(0)->size);
    
    // traversal, and print out huffman encoding
    traversal(tree.at(0),std::string(""));

    // we can construct the huffman tree - encode

    // print 
    /*for(std::vector<huffman_tree>::iterator it=tree.begin(); it!=tree.end(); ++it){
        printf("%c: %d\n", (*it).orig, (*it).size);
    }*/

    printf("=================================================================\n");

    printf("Compressed bits: %d, Original bits: %d\n", compressed_bits, original_bits);
    printf("Save memory size: %f %%\n", (original_bits-compressed_bits)*100/(double)original_bits);

    return 0;
}