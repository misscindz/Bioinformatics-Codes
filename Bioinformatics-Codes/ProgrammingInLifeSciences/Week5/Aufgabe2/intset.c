#include "intset.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>

#define D 65536
#define MIN(a,b) a<b?a:b


struct IntSet{
 unsigned long *original,
 			      *sectionstart,
 			      sectionstart_length,
 			      *sectionlength, 
               max_elem,
               elem_count, 
               max_val;
 uint16_t *elements;
};

IntSet *intset_new(unsigned long maxvalue, unsigned long nofelements){
   int i;
   
   IntSet *new_intset = malloc(sizeof(*new_intset));
   assert(new_intset != NULL);
   new_intset->original = malloc(sizeof(unsigned long)*nofelements);
   assert(new_intset->original != NULL);
   new_intset->elements = malloc(sizeof(unsigned long)*nofelements);
   assert(new_intset->elements != NULL);
   new_intset->sectionstart_length = (maxvalue/D)+2;
   new_intset->sectionstart = malloc(new_intset->sectionstart_length*sizeof(unsigned long));
   assert(new_intset->sectionstart != NULL);
   new_intset->sectionlength = malloc(new_intset->sectionstart_length*sizeof(unsigned long));
   assert(new_intset->sectionlength != NULL);
   
   //initialize elem to 0
   for (i=0; i<nofelements; i++){
      new_intset->original[i] = 0;
   }
   for (i = 0; i < new_intset->sectionstart_length; i++){
      new_intset->sectionstart[i] = 0;
      new_intset->sectionlength[i] = 0;
   }
   new_intset->max_elem = nofelements;
   new_intset->elem_count = 0;
   new_intset->max_val = maxvalue;
   return new_intset;
}

size_t intset_size(unsigned long maxvalue, unsigned long nofelements){
   return sizeof(unsigned long)*nofelements;
}

void intset_delete(IntSet *intset){
   free(intset->original);
   free(intset->elements);
   free(intset->sectionstart);
   free(intset->sectionlength);
   free(intset);
}

void intset_add(IntSet *intset, unsigned long elem){
   int key, i, j, q, previous_quotient;
       
   if (elem > intset->max_val){
      fprintf(stderr,"element is larger than maximum value\n");
      exit(EXIT_FAILURE);
   }
   else if (intset->max_elem == intset->elem_count){
      fprintf(stderr,"maximum number of elements reached\n");
      exit(EXIT_FAILURE);
   }
   else{
      intset->original[intset->elem_count] = elem;
      //insertion-sort
      
      for (i = 1, j = i-1; i < intset->elem_count; i++){
         key = intset->original[i];
         while((j > 0) && (intset->original[j] > key)){
            intset->original[i] = intset->original[j];
            j--;
            intset->original[j+1] = key;
         }
      }
      intset->elem_count++;
      (intset->sectionlength[intset->original[j]/D])++;	
      
      for (j = 0; j < intset->elem_count; j++){
      	
      	intset->elements[j] = (intset->original[j])%D;
      	q = intset->original[j]/D;
      	
      	if (j == 0){    		
      		intset->sectionstart[q] = j; 
      	}
      	
      	else{
      		previous_quotient = intset->original[j-1]/D;
      		if (elem>50000){
      		printf("previous quotient= %d, current quotient=%d", previous_quotient, q);
      		}
      		if (previous_quotient < q){
      			printf("%d\n", j);
      			intset->sectionstart[q] = j;
      		} 
      		else{
      		   printf("%d\n", j);
      		   intset->sectionstart[q] = intset->sectionstart[previous_quotient];
      		}
      	}

      }
    }
}


bool intset_is_member(const IntSet *intset, unsigned long elem){
	int i, j;
	if (intset->sectionlength[elem/D] == 0 ) {
      //element found
      return false;
   }
   else{
    	for(i=0, j=intset->sectionstart[elem/D]; i<(intset->sectionlength[elem/D]); i++, j++){
		//	printf("aktuelles Element: %lu \t loop i: %d \t index %d \t sectionlength: %lu\n", intset->original[j], i,j, intset->sectionlength[elem/D]);
			
			if (intset->original[j] == elem){
				if (elem>50000){
			      printf("aktuelles Element: %lu \t loop i: %d \t index %d \t sectionlength: %lu\t sectionstart: %lu\n", intset->original[j], i,j, intset->sectionlength[intset->original[j]/D], intset->sectionlength[intset->original[j]/D]);
			   }
   
		//	printf("\nq= %lu, sectionlength[%lu] = %lu, original[%d] = %lu", elem/D, elem/D,intset->sectionlength[elem/D], j, intset->original[j]);	
			// 	printf("found\n");
				return true;
			}
		}
   }
   //element not found  
   return false;
}

unsigned long intset_number_next_larger(const IntSet *intset,
                                        unsigned long pos){
   return 0;                                       
}

/* Print out the <intset> to stdout */
void intset_pretty(const IntSet *intset){
   unsigned int i;
   for (i = 0; i < intset->elem_count; i++){
      printf("%lu ", intset->original[i]);
   }
}

