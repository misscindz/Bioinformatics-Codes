// Blatt 2 Aufgabe 2
// Gruppe: Achoukhi, Breitschuh, Ching

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

typedef struct{
  unsigned long start_in_seq_1, 
                start_in_seq_2, 
                end_in_seq_1,
                end_in_seq_2,
                weight,
                score,
                prec,
                chain_len; 
} match;


match *read_matches_file(char *filename, unsigned long *num_of_matches){

  FILE *match_file;
  unsigned long s1, 
  				e1,
  				s2, 
  				e2, 
  				w;
  unsigned long reserved_matches = 10;
  match *matches = malloc(reserved_matches * sizeof(*matches));
  
  assert(matches != NULL);
  *num_of_matches = 0;
  match_file = fopen(filename,"r");
  
  if (match_file == NULL){
    fprintf(stderr,"Error opening match file %s!\n", filename);
    exit(EXIT_FAILURE);
  }
  
  while (fscanf(match_file,"%lu\t%lu\t%lu\t%lu\t%lu\n",&s1,&e1,&s2,&e2,&w) == 5){   
    if (*num_of_matches >= reserved_matches){
      reserved_matches *= 2;
      matches = realloc(matches, reserved_matches * sizeof(*matches));
      assert(matches != NULL);
    }
    matches[*num_of_matches].start_in_seq_1 = s1;
    matches[*num_of_matches].start_in_seq_2 = s2;
    matches[*num_of_matches].end_in_seq_1 = e1;
    matches[*num_of_matches].end_in_seq_2 = e2;
    matches[*num_of_matches].weight = w;
    (*num_of_matches)++;
  }
  fclose(match_file);
  return matches;
}


int comp_matches (const void *a, const void *b){
  return ((*(match*)a).end_in_seq_1 >= (*(match*)b).end_in_seq_1);
}


void simple_chaining (	match *matches, 
						unsigned long num_of_matches,
                  		unsigned long seq1_len, 
                  		unsigned long seq2_len){
                  		
  unsigned long maxscore, overallmaxscore = 0;
  unsigned long bestmatch = 0;
  unsigned long i, j;
  unsigned long chain_len, bp_len_1, bp_len_2, next_match;
  unsigned long *optimal_chain;
  
  qsort(matches, num_of_matches, sizeof(match), comp_matches); 
  
  for (i = 0; i < num_of_matches; i++){
  	maxscore = matches[i].weight;
  	matches[i].chain_len = 1;
    for (j = 0; j < i; j++){
      if (	matches[j].end_in_seq_1 < matches[i].start_in_seq_1 
      		&& matches[j].end_in_seq_2 < matches[i].start_in_seq_2 
      		&& maxscore < matches[j].score + matches[i].weight){    
        maxscore = matches[j].score + matches[i].weight;
        matches[i].chain_len = matches[j].chain_len + 1;
       	matches[i].prec = i;
      }
    }
    matches[i].score = maxscore;
    if (overallmaxscore < maxscore){
      overallmaxscore = maxscore;
      bestmatch = i;
    }
  }
  
  chain_len = matches[bestmatch].chain_len;
  optimal_chain = malloc(chain_len*sizeof(unsigned long));
  assert(optimal_chain != NULL);  
  optimal_chain[chain_len-1] = bestmatch;
  bp_len_1 = 0;
  bp_len_2 = 0;
  next_match = bestmatch;
  
  for (i = 1; i <= chain_len; i++){
    optimal_chain[chain_len - i] = next_match;
    bp_len_1 += matches[next_match].end_in_seq_1 - matches[next_match].start_in_seq_1 + 1;
    bp_len_2 += matches[next_match].end_in_seq_2 - matches[next_match].start_in_seq_2 + 1;
    next_match = matches[next_match].prec;
  }
  
  for (i = 0; i < chain_len; i++){
    j = optimal_chain[i];
    printf("%lu %lu %lu %lu %lu\n", matches[j].start_in_seq_1,
                                    matches[j].end_in_seq_1,
                                    matches[j].start_in_seq_2,
                                    matches[j].end_in_seq_2,
                                    matches[j].weight);
  }
  
  printf("# optimal chain of length %lu with score %lu\n", 
         					chain_len, overallmaxscore);
  printf("# %lu bp covered on sequence 1 (coverage %.2lf%%)\n", 
         					bp_len_1, 100.0*(double)bp_len_1/(double)seq1_len);
  printf("# %lu bp covered on sequence 2 (coverage %.2lf%%)\n", 
         					bp_len_2, 100.0*(double)bp_len_2/(double)seq2_len);
  free(optimal_chain);
}


int main(int argc, char *argv[]) {
  
  char *filename;
  match *matches;
  unsigned long num_of_matches;
  unsigned long seq1_len, seq2_len;
  
  if (argc != 4 ||sscanf(argv[1],"%lu",&seq1_len) != 1 
  				||sscanf(argv[2],"%lu",&seq2_len) != 1){				
    fprintf(stderr, "Usage: %s <length s1> <length s2> <match file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  filename = argv[3];
  matches = read_matches_file(filename, &num_of_matches);
  simple_chaining(matches, num_of_matches, seq1_len, seq2_len);
  
  return EXIT_SUCCESS;
}
