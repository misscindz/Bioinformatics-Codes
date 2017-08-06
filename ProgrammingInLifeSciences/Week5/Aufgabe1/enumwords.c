#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"


void tokenhandler(const char *word, void *data){
    
    printf("%s", word);

}

int main(int argc, char *argv[]){
    
    FILE *fs;
    char filename[100];
    
    if (argc<2){
        fprintf(stderr,"Usage: %s <filename>", argv[0]);
        return EXIT_FAILURE;
    }
    if ((sscanf(argv[1],"%s", filename)!= 1)){
        fprintf(stderr,"Error filename format");
        return EXIT_FAILURE;
    }
    
    fs = fopen(filename,"rb");
    if (fs == NULL){
        fprintf(stderr, "%s: error in fopen\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    tokenizer(fs, tokenhandler, 0);
    
    fclose(fs);
    
    return EXIT_SUCCESS;
}