//Aufgabe 1
//Gruppe: Achoukhi, Wang, Ching

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[]){
    long filesize;
    int i = 0, j= 0,k =0, l= 0;
    char *buffer = NULL;
    char c;
    char *output;
    int *newline_pos = NULL;
    if(argc != 2){
        fprintf(stderr,"Usage: %s <filename>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *r = fopen(argv[1],"r");
    assert(r!=NULL);
    output = strcat(argv[1],".rc");
    FILE *w = fopen(output,"w");
    assert(w != NULL);
    
    fseek (r, 0, SEEK_END);
    filesize = ftell(r);
    assert(filesize >= 0);
    buffer = malloc(sizeof(char) * (filesize+1));
    assert(buffer != NULL);
    newline_pos = malloc(sizeof(int) *(filesize+1));
    assert(newline_pos != NULL);
    fseek(r,0,SEEK_SET);
    
    while (!feof(r)) {
        c = getc(r);
        if(c == '\n'){
            newline_pos[k] = i;
            k++;
            continue;
        }

            buffer[i] = c;
            ++i;
    }
    
    k = 0;
    for(l = i-2; l>= 0; --l){
        
        if(j == newline_pos[k]){
            fprintf(w,"\n");
            k++;
            
        }

        if(buffer[l]=='A'){
          buffer[l] = 'T';
          j++;
        }
        else if (buffer[l] == 'T'){
          buffer[l] = 'A';
          j++;
        }
        else if (buffer[l] == 'G'){
          buffer[l] = 'C';
          j++;

        }
        else if(buffer[l] == 'C'){
          buffer[l] = 'G';
          j++;
        }
        
        fprintf(w,"%c",buffer[l]);

    }
    
    fprintf(w,"\n");
    fclose(r);
    fclose(w);
    free(buffer);
    free(newline_pos);
    return EXIT_SUCCESS;
}
