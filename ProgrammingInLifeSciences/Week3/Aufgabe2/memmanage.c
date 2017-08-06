// Gruppe: Achoukhi, Wang, Ching
// PFN Blatt03 Aufgabe 2
// Calculation of Reversed Partial sums

#include <stdio.h>
#include <stdlib.h>
#include "memmanage.h"
#include <assert.h>
#include "memmanage-mac.h"
#include <stdbool.h>
#include <string.h>

struct MMspaceblock{
  void *blockptr;
  unsigned long size, 
                num_of_cells,
                line;
  char *file;
};

struct MMspacetable{
  MMspaceblock *block;
  unsigned long total_blocks, active_block;
};

MMspacetable *mem_man_new(unsigned long numberofblocks){
  int i;
  MMspacetable *st = malloc(sizeof(MMspacetable));
  assert(st!=NULL);
  st->block = malloc (sizeof(MMspaceblock)*numberofblocks);
  assert(st->block!=NULL);
  st->active_block=0;
  for (i=0; i<numberofblocks; i++){
    st->block[i].blockptr=NULL;
  }  
  st->total_blocks = numberofblocks;
  return st;
}

void *mem_man_alloc(MMspacetable *st, char *file, unsigned long line,
				    void *ptr, unsigned long size, unsigned long number){

  st->block[st->active_block].blockptr=realloc(ptr, size*number);
  st->block[st->active_block].size = size;
  st->block[st->active_block].num_of_cells = number;
  st->block[st->active_block].line = line;
  st->block[st->active_block].file = file;
  st->active_block++;
  
  // for (i=0; i<st->num_of_blocks; i++){
  //   st->block[i]->size = size;
  //   st->block[i]->num_of_cells = sizeof(st->block[i]->blockptr)/size;
  //   st->block[i]->line = line;
  //   st->block[i]->file = file;
  // }
  return st->block[st->active_block].blockptr;
}

void mem_man_delete_ptr(MMspacetable *st, char *file, unsigned long line,
				    void *ptr){
  int i;
  for(i=0; i<=st->active_block; i++){
    if (st->block[i].blockptr == ptr){
      free(st->block[i].blockptr);
      st->block[i].size =0;
      st->block[i].num_of_cells = 0;
      st->block[i].line = line;
      st->block[i].file = file;
    }
    else{
      fprintf(stderr,"Memory not allocated yet\n");
      exit(EXIT_FAILURE);
    }
  }
}

void mem_man_info(const MMspacetable *st){
  int i;
  printf("Print out internal memory table:\n");
  for(i=0; i<st->total_blocks; i++){
    if (st->block[i].blockptr != NULL){
      printf("# active block %d: allocated in file \"%s\", line %lu\n", 
              i, st->block[i].file, st->block[i].line);
    }
  }
}

void mem_man_check(const MMspacetable *st){
  int i;
  for(i=0; i<st->total_blocks; i++){
    if (st->block[i].blockptr != NULL){
      fprintf(stderr,"space leak: memory for block %d not freed\n%lu cells of size %lu\nallocated: file \"%s\", line %lu\n", i, st->block[i].num_of_cells, st->block[i].size, st->block[i].file, st->block[i].line);
      exit(EXIT_FAILURE);
    }
  }
}

void mem_man_delete(MMspacetable *st){
  free(st->block);
  free(st);
}
