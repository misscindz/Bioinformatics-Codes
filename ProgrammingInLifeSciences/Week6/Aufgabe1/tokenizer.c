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
        if (  ((isspace((int)cc))&&(tokenidx > 0))
            ||
              (((40 == (int)cc))&&(tokenidx > 0))
            ||
              (((41 == (int)cc))&&(tokenidx > 0))
            ||
              (((45 == (int)cc))&&(tokenidx > 0))
            ||
              (((62 == (int)cc))&&(tokenidx > 0))
            ||
              (((44 == (int)cc))&&(tokenidx > 0))
            ||
              (((42 == (int)cc))&&(tokenidx > 0))
            ||
              (((37 == (int)cc))&&(tokenidx > 0))
            ||
              (((58 == (int)cc))&&(tokenidx > 0))
            ||
              (((92 == (int)cc))&&(tokenidx > 0))
            ||
              (((46 == (int)cc))&&(tokenidx > 0))
            ||
              (((34 == (int)cc))&&(tokenidx > 0))
            ||
              (((91 == (int)cc))&&(tokenidx > 0))
            ||
              (((93 == (int)cc))&&(tokenidx > 0))
            ||
              (((32 == (int)cc))&&(tokenidx > 0))
           )
        {
            tokenhandler(token, data);
            tokenidx = 0;
            free(token);
            token = calloc(1,sizeof(char));
            loop_count = 0;
        }

        loop_count++;
    }
    free(token);
}

