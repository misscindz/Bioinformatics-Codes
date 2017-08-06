// Gruppe: Achoukhi, Wang, Ching
// PFN Blatt03 Aufgabe 2
// Calculation of Reversed Partial sums

#include <stdio.h>
#include <stdlib.h>

void revert_partialsums (long *ps, unsigned long n){
  int i;
  ps[0]=0;
  for (i=0; i<n-1; i++){ 
    ps[i] = ps[i+1] - ps[i];
    printf("%d: %ld\n",i , ps[i]);
  }
}

int main(int argc, char *argv[]){
 int k;
 long num, ps[argc+1];

  if (argc<2){
    fprintf(stderr,"Usage: <program> <array of numbers separated by space>");
    return EXIT_FAILURE;
  }
  
  for (k=1; k<argc; k++){
    if (!(sscanf(argv[k],"%ld", &num))){
      fprintf(stderr,"enter number");
      return EXIT_FAILURE;
    }
    ps[k]=num;
  }
  
  revert_partialsums(ps, argc);
  return EXIT_SUCCESS;
}