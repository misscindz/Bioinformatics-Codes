// Wang, Ching, Achoukhi
// Blatt 8 Aufgabe 1

#ifndef MULTISEQ_H
#define MULTISEQ_H

#include <stdio.h>


typedef struct Seqinfo{
    char            *seqstart;
    unsigned long   seqlen, 
                    seqno;
    
}Seqinfo;

typedef struct Seqpair{
    Seqinfo seq1;
    Seqinfo seq2;
    long    distance;
}Seqpair;

typedef struct Multiseq{
    char     **seqs;
    Seqinfo  *seqinfo;
    long     numofseqs;
    Seqpair  **pairtable;
}Multiseq;

void save_sequences(FILE *file, Multiseq *sequences, char *filename);

#endif
