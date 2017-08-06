// Wang, Ching, Achoukhi
// Blatt 8 Aufgabe 1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <stdbool.h>

#include "multiseq.h"



void save_sequences(FILE *file, Multiseq *sequences, char *filename){
    char c;
    long i=0,j=0;
    bool inseq = false;

    sequences->seqs = malloc(300*sizeof(*sequences->seqs));
    assert(sequences->seqs != NULL);
    sequences->seqinfo = malloc(300*sizeof(*sequences->seqinfo));
    assert(sequences->seqinfo != NULL);
    sequences->numofseqs=0;
   
    file = fopen(filename, "r");
    assert(file != NULL);
    
    
    while(!feof(file)){
        
        c = fgetc(file);
        if (c == '\n'){
            inseq = true;
            continue;
        } 
        if (c == '>'){
            inseq = false;
            i++;
            j=0;
            sequences->seqinfo[i-1].seqstart = sequences->seqs[i-1];
            sequences->seqinfo[i-1].seqno = i-1;
            sequences->numofseqs++;
        }
        if (inseq){
            sequences->seqs[i-1]=(char *)malloc(sizeof(char)*5000);
            sequences->seqs[i-1][j] = c;
            sequences->seqinfo[i-1].seqlen++; 
        }
    }
    
    fclose(file);
}