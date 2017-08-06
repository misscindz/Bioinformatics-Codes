// PFN: Blatt6, Aufgabe2
// Gruppe: Wang, Achoukhi, Ching
// Abgabedatum: 2.6.16

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>


int main(int argc, char *argv[]){
  int maxsteps, na, nb, size;
  unsigned int i=0, j;
  double pa, pb, number; 
  bool createfile=false;
  FILE *file;
  srand48(time(NULL));

  
  if (argc == 6){
    if(sscanf(argv[1], "%d",&na)!= 1 
                        || sscanf(argv[2], "%lf",&pa)!= 1
                        || sscanf(argv[3],"%d",&nb)!= 1 
                        || sscanf(argv[4],"%lf",&pb)!= 1
                        || sscanf(argv[5],"%d",&maxsteps)!= 1){
      fprintf(stderr,"Usage: %s <starting population dolly A>", argv[0]);
      fprintf(stderr,"\n<splitting probability dolly A as decimal number, e.g. 0.3>");
      fprintf(stderr,"\n<starting population dolly B>");
      fprintf(stderr,"\n<splitting probability dolly B as decimal number, e.g. 0.3>");
      fprintf(stderr,"\n<number of steps>");
      exit(EXIT_FAILURE);
    }
  }
  
  else if (argc == 7 ) {
    if(sscanf(argv[1], "%d",&na)!= 1 
                        || sscanf(argv[2], "%lf",&pa)!= 1
                        || sscanf(argv[3],"%d",&nb)!= 1 
                        || sscanf(argv[4],"%lf",&pb)!= 1
                        || sscanf(argv[5],"%d",&maxsteps)!= 1){
      fprintf(stderr,"Usage: %s <starting population dolly A>", argv[0]);
      fprintf(stderr,"\n<splitting probability dolly A as decimal number, e.g. 0.3>");
      fprintf(stderr,"\n<starting population dolly B>");
      fprintf(stderr,"\n<splitting probability dolly B as decimal number, e.g. 0.3>");
      fprintf(stderr,"\n<number of steps>");
      exit(EXIT_FAILURE);
    }
         
      file = fopen(argv[6],"w+");
      if(file == NULL){
        printf("can not open the file to write\n");
        exit(EXIT_FAILURE);
      }
      createfile = true;
      fprintf(file,"nstep\tnA\tnB\n");
  }
            
  else{
    fprintf(stderr,"Usage: %s <starting population dolly A>", argv[0]);
    fprintf(stderr,"\n<splitting probability dolly A as decimal number, e.g. 0.3>");
    fprintf(stderr,"\n<starting population dolly B>");
    fprintf(stderr,"\n<splitting probability dolly B as decimal number, e.g. 0.3>");
    fprintf(stderr,"\n<number of steps>");
    exit(EXIT_FAILURE);
  }
 
  size = na + nb;
  
  while (i<maxsteps){
    
    number = drand48();
    if (pa>number){
      na++;
    }
    if (pb>number){
      nb++;
    }
    for(j=na+nb; j>size; --j){
      number = drand48();
      if(na>0 && nb>0){
        if (0.5<=number){
          nb--;
        }
        else{
          na--;
        }
      }
    }
    if (createfile){
      fprintf(file,"%u\t%d\t%d\n",i,na,nb);
    }
        
    if (na == 0){
      printf("fixed:B\tsteps:%u\n",i);
      break;
    }
    if(nb == 0){
      printf("fixed:A\tsteps:%u\n",i);
      break;
    }
    i++;
  }
  

  if(na != 0 && nb != 0){
    printf("simulation stopped after %d steps",maxsteps);
    printf(" (A:%ld,B:%ld)\n",(long int)na, (long int) nb);
  }
  
  if(createfile){
    fclose(file);
  }
  return EXIT_SUCCESS;
  
}