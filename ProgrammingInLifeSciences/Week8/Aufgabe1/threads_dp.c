// Wang, Ching, Achoukhi
// Blatt 8 Aufgabe 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "bestkvals_generic.h"
#include "multiseq.h"
#include "unit_edist.h"


typedef struct Infotab{
    pthread_t   ident;
    Multiseq    *sequences;
    Calc_edist  eval_unit_edist;
    long        section_start, 
                section_end;
    BestKVals   *bkv;
}Infotab;

int compare(const void *a, const void *b){
    return (*((int*)a) - (*((int*)b)));
}
        
void *eval_seqrange(void *thread_inf){
    
    Infotab *thread_infos = (Infotab *)thread_inf;
    int l = 0;
    long n, m, k;
    
    n = thread_infos->sequences->numofseqs;
    m = thread_infos->section_start;
    k = thread_infos->section_end;
    
    for(l=0; l<n; l++){
        //pro thread nur von n bis k
        for(m=0; m<k; m++){
            
            thread_infos->sequences->pairtable[l][m].seq1.seqstart 
                                    = thread_infos->sequences->seqs[l];
            thread_infos->sequences->pairtable[l][m].seq2.seqstart 
                                    = thread_infos->sequences->seqs[m];
            thread_infos->sequences->pairtable[l][m].seq1.seqlen 
                                    = thread_infos->sequences->seqinfo[l].seqlen;
            thread_infos->sequences->pairtable[l][m].seq2.seqlen 
                                    = thread_infos->sequences->seqinfo[l].seqlen;
            thread_infos->sequences->pairtable[l][m].seq1.seqno 
                                    = l;
            thread_infos->sequences->pairtable[l][m].seq2.seqno 
                                    = m;
            thread_infos->sequences->pairtable[l][m].distance 
                        = thread_infos->eval_unit_edist(
                        (const unsigned char *)thread_infos->sequences->seqs[l],
                        thread_infos->sequences->seqinfo[l].seqlen,
                        (const unsigned char *)thread_infos->sequences->seqs[m],
                        thread_infos->sequences->seqinfo[m].seqlen);
            best_k_vals_insert(thread_infos->bkv, 
            (const void *)&(thread_infos->sequences->pairtable[l][m]));
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    
    Multiseq *sequences = (Multiseq *)malloc(sizeof(*sequences));
    assert(sequences != NULL);
    
    Seqpair *print_seq = (Seqpair *)malloc(sizeof(*print_seq));
    assert(print_seq != NULL);
    
    FILE *f=NULL;
    int t=0, k=0, i,j, next_section=0, section_size=0;
    Infotab *thread_infos;
    BestKVals *bkv;
    
    if(argc != 4){
        fprintf(stderr, "Usage: %s <filename> <k> <t> <fasta file>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    else{
        if (sscanf(argv[1], "%d", &k) != 1 || k < 1){
            fprintf(stderr, "Could not read <k>!\n");
            exit(EXIT_FAILURE);
        }
        if (sscanf(argv[2], "%d", &t) != 1 || t < 1){
            fprintf(stderr, "Could not read <t>!\n");
            exit(EXIT_FAILURE);
        }

    }
    // sequences->pairtable = malloc((k*200)*sizeof(sequences->pairtable));
    // assert(sequences->pairtable != NULL);
    //macht neuen bkv
    bkv = best_k_vals_new(k,compare, sizeof(sequences->pairtable));
    
    //liest datei ein und speichert sequenzen in array
    save_sequences(f,sequences,argv[3]);

    
    //Infotab initialisieren + aufteilen in Sections
    thread_infos = malloc(t* sizeof(*thread_infos));
    assert(thread_infos != NULL);
    thread_infos->sequences = malloc(sizeof(Multiseq *));
    assert(thread_infos->sequences != NULL);
    
    section_size = (sequences->numofseqs)/t;
    
    for (i=0; i<t; i++){
        thread_infos[i].sequences = sequences;
        thread_infos[i].eval_unit_edist = eval_unit_edist;
        next_section += section_size; 
        thread_infos[i].section_start = next_section;
        thread_infos[i].section_end = next_section + section_size;
        thread_infos[i].bkv = bkv;
    }
    
    //threads creation and joining
    for (j=0; j<t; j++){
        pthread_create(&thread_infos[j].ident, NULL, 
                                eval_seqrange, thread_infos+j);
        pthread_join(thread_infos[j].ident, NULL);
    }
    
    //print best k vals
    for (i=0; i<k; i++){
        print_seq = (Seqpair *)best_k_vals_next(bkv);
        printf("%lu\t%lu\t%lu\n", print_seq->seq1.seqno,
                                print_seq->seq2.seqno,
                                print_seq->distance);
    }
    
    free(thread_infos);
    free(sequences->pairtable);
    
    for (i=0; i<sequences->numofseqs; i++){
        free(sequences->seqs[i]);
    }
    free(sequences->seqinfo);
    free(sequences);
    free(print_seq);
    
    return EXIT_SUCCESS;
    
}