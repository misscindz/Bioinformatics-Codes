#include "intset.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

struct IntSet{
 unsigned long *elements, 
               max_elem,
               elem_count, 
               max_val;
};

IntSet *intset_new(unsigned long maxvalue, unsigned long nofelements){
   int i;
   
   IntSet *new_intset = malloc(sizeof(*new_intset));
   assert(new_intset != NULL);
   new_intset->elements = malloc(sizeof(unsigned long)*nofelements);
   assert(new_intset->elements != NULL);
   
   //initialize elem to 0
   for (i=0; i<nofelements; i++){
      new_intset->elements[i] = 0;
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
   free(intset->elements);
   free(intset);
}

void intset_add(IntSet *intset, unsigned long elem){
   int key, i, j;
       
   if (elem > intset->max_val){
      fprintf(stderr,"element is larger than maximum value\n");
      exit(EXIT_FAILURE);
   }
   else if (intset->max_elem == intset->elem_count){
      fprintf(stderr,"maximum number of elements reached\n");
      exit(EXIT_FAILURE);
   }
   else{
      intset->elements[intset->elem_count] = elem;
      //insertion-sort
      for (i = 1, j = i-1; i < intset->elem_count; i++){
         key = intset->elements[i];
         while((j > 0) && (intset->elements[j] > key)){
            intset->elements[i] = intset->elements[j];
            j--;
            intset->elements[j+1] = key;
         }
      }

      intset->elem_count++;
   }
}


bool intset_is_member(const IntSet *intset, unsigned long elem){
   unsigned long left_idx, right_idx, mid;
   left_idx = 0;
   right_idx = intset->elem_count-1;
   while ((left_idx <= right_idx) && (left_idx >= 0) && 
                                 (right_idx < intset->elem_count)){
      mid = ((right_idx-left_idx)/2)+left_idx;

      if ((elem == intset->elements[mid])){
         //element found
         return true;
      }
      else if (elem < intset->elements[mid]){
         right_idx = mid-1;
      }
      else{
         left_idx = mid+1;
      }

   }
   //element not found  
   return false;
}

const unsigned long intset_number_next_larger(const IntSet *intset,
                                        unsigned long pos){
                                           
   return 0;                                       
}

/* Print out the <intset> to stdout */
void intset_pretty(const IntSet *intset){
   unsigned int i;
   printf("\n");
   for (i = 0; i < intset->elem_count; i++){
      printf("%lu ", intset->elements[i]);
   }
}

