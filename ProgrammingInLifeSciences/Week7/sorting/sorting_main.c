// Wang, Ching, Achoukhi
// Blatt 7 Aufgabe 3
// Countingsort besitzt eine Laufzeit O(n+k). Wenn k = O(n) gilt, so besitzt 
// Countingsort eine Laufzeit cvon O(n).
// insertionsort besitzt eine Laufzeit von O(n), wenn das Eingabearray bereits
// soritert ist. Wenn nicht, dann O(n²).

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "sorting.h"


#define MAXARRAYELEMENTS 1000000
#define MAXSORTVALUE 1000U

int main(int argc, char *argv[])
{   
    unsigned int idx, i, j, k=0, resume_cnt = 0;  
    unsigned int count[10], dezentil[10], nofelements = 0; 
    unsigned int *values, *values_copie;

    if(argc != 2)
    {
      fprintf(stderr,"Usage: <Anzahl der zufälligen Zahlen>\n");
      return EXIT_FAILURE;      
    }
    
    sscanf(argv[1],"%d", &nofelements);
    
    if (nofelements > MAXARRAYELEMENTS || nofelements < 0)
    {
        fprintf(stderr,"Usage: <Anzahl der zufälligen Zahlen > 0 und < 1000000 >\n");
        return EXIT_FAILURE;
    }
        
    values = malloc(sizeof(unsigned int)*nofelements);
    values_copie = malloc(sizeof(unsigned int)*nofelements);
    assert(values != NULL);
    assert(values_copie != NULL);
    
    printf("# Erzeugung der Zufallszahlen...\n");
    printf("# Kopieren der gleichen Zufallszahlen...\n");    
    
    srand48(42349421);
    for (i=0; i < nofelements; i++)
    {
        values[i] =(unsigned int) (drand48() * (MAXSORTVALUE+1));
        values_copie[i] = values[i];
    }
    
    insertionsort(values,nofelements);
    countingsort(values_copie,nofelements);
    
    printf("# Vergleich der Arrays...\n");

    for (j = 0; j < nofelements; j++)
    {
      if (values[j] != values_copie[j])
      {
        fprintf(stderr,"Error im Vergleich\n");
        return EXIT_FAILURE;
      }    
    }
    
    printf("# Array identisch!\n");
    
    //Dezentile initialisieren
    for(idx =0; idx < 10; idx++)
    {
        dezentil[idx] = nofelements/10*(idx+1);
    }
    
    printf("Kleinste erzeugte Zufallszahl: %d\n", values[0]);
    printf("Groesste erzeugte Zufallszahl: %d\n",values[nofelements-1]);
    
    //Counter initialisieren 
    for (i = 0; i < 10; i++)
    {
      count[i] = 0;    
    }
    
    for(k = values[resume_cnt]; k < nofelements; k++)
    {
      for (i= 0; i < 10; i++)
      {  
        if(values[k] <= dezentil[i])
        {
          count[i]++;
          break;
        }   
        else 
        {
          if(values[k] > dezentil[i])
          {
            resume_cnt = dezentil[i]+1;  
            continue;
          }
        }
      }
    }

    printf("Dezentilverteilung:\n\n");
    printf("Dezentil\t#Werte\n");
    printf("_________________________\n");
    printf("%u-%u\t\t %u\n",0,dezentil[0], count[0]);    
    for (k = 0; k < 9; k++)
    {
      printf("%u-%u \t %u\n",dezentil[k]+1,dezentil[k+1], count[k+1]);    
    }
    
    free(values);
    free(values_copie);
    return EXIT_SUCCESS;
}
