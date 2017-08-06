#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"
#include <ctype.h>

void tokenizer (FILE *fp, TokenHandlerFunc tokenhandler, void *data){
    
    char *token = malloc(sizeof(char));
    char cc;
    int loop_count = 0, tokenidx = 0;
    
    while ((cc = getc(fp)) != EOF){
        
        if (loop_count > 0){
            token = realloc(token, loop_count*sizeof(char));
        }
        if (((48 <= (int)cc)&&((int)cc<=57))||
                            ((65 <= (int)cc)&&((int)cc <=90))||
                            ((48 <= (int)cc)&&((int)cc <=57))||
                            ((97 <= (int)cc)&&((int)cc <=122))||
                            ((int)cc == 95)){
            token[tokenidx++] = cc;
        }
        if ((isspace((int)cc))&&(tokenidx > 0)){
            tokenhandler(token, data);
            printf("\n");
            tokenidx = 0;
            free(token);
            token = calloc(1,sizeof(char));
        }
        loop_count++;
    }
    free(token);
}

