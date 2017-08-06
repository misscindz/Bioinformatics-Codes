/* PBI Aufgabe 6.2
 *
 * Ching,Achoukhi,Wang
 * Abgabe: 23.11.2015 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void show_sorted_reverse (const int *a, int alen, const int *b, int blen)
{
  int aidx = alen-1 , bidx = blen-1;
  
  while (1){ 
    if (aidx >= 0){
      if (bidx < 0 || a[aidx] > b[bidx])
        printf ("%d\n", a[aidx--]);
      else
        printf ("%d\n", b[bidx--]);
    }
    else{
      if (bidx >= 0)
        printf ("%d\n", b[bidx--]);
      else
        return;
    }
  }
}


int main ()
{ 
  int liste_a [] = {0,3,5,12,16,35};
  int liste_b [] = {1,3,13,15,22,34,42};
  int alen, blen;
  
  alen = sizeof (liste_a) / sizeof (liste_a[0]);
  blen = sizeof (liste_b) / sizeof (liste_b[0]);
  show_sorted_reverse (liste_a,alen,liste_b,blen);
  
return EXIT_SUCCESS;
} 


