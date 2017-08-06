#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "sorting.h"

#define MAXARRAYELEMENTS 1000000
#define MAXSORTVALUE 1000U

/* Function to sort an array using insertion sort*/
void insertionsort(unsigned int *values, unsigned long nofelements)
{
   int i, j;
   unsigned int key;

   printf("# Sortieren der Zufallszahlen mit Insertion Sort...\n");
   
   for (i = 1; i < nofelements; i++)
   {
       key = values[i];
       j = i-1;
       
       while (j >= 0 && values[j] > key)
       {
           values[j+1] = values[j];
           j = j-1;
       }
       values[j+1] = key;
   }
}

void countingsort(unsigned int *values, unsigned long nofelements)
{
  int i, j, k, l, m;

  unsigned int *output = malloc(sizeof(unsigned int) *nofelements);
  assert( output != NULL && "Memmory allocation failed!");

  unsigned int *count = malloc(sizeof(unsigned int) *(MAXSORTVALUE+1));
  assert( count != NULL && "Memmory allocation failed!");

  printf("Sortieren der Zufallszahlen mit Counting Sort...\n");
  
  for(i=0; i <= MAXSORTVALUE; i++)
  {
    count[i] = 0;
  }
  
  for(j=0; j < nofelements; j++)
  {
    count[values[j]] = count[values[j]] + 1;
  }
  
  for(k=1; k <= MAXSORTVALUE; k++)
  {
    count[k] = count[k] + count[k-1];
  }
  
  for (l=0; l < nofelements; l++)
  {
    output[count[values[l]]-1] = values[l];
    count[values[l]]--;
  }
  
  for(m=0; m<nofelements; m++)
  {
    values[m] = output[m];
  }

}

