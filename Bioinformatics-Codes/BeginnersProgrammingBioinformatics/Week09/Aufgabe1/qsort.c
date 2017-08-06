/*PBI
Aufgabe 9.1
Gruppe: Achoukhi, Wang, Ching
Abgabe 14. Dezember 2015
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define NUMOFENTRIES 1000

typedef struct {
    
    long int *index;

} Interval;

   static int compareIntervals_auf (const void *keya, const void *keyb){

    const long int *ia = (const long int *) keya;
    const long int *ib = (const long int *) keyb;
    if (*ia < *ib) {
        return -1;
    }
    if (*ia > *ib) {
        return 1;
    }
    return 0;
}

    static int compareIntervals_ab (const void *keya, const void *keyb){
        
        const long int*ia = (const long int *) keya;
        const long int *ib = (const long int *) keyb;
        if (*ia < *ib) {
            return 1;
        }
        if (*ia > *ib) {
            return -1;
        }
        return 0;
    }

    static void array_show(const long *arr, unsigned long numofentries) {
    
        const long *ptr;
        for (ptr = arr; ptr < arr + numofentries; ptr++) {
        printf("%ld\n",*ptr);
        }
    }
    
   int main(int argc, char **argv){
    
    Interval sort;
    unsigned int idx=0;
    char* a={"-a"};
    char* d={"-d"};
    char row[80];
   
       if(argc!=3){
        
        fprintf(stderr,"no input number\n");
        fprintf(stderr,"Usage:%s please input character for operation and the name of the datei",argv[0]);
        return EXIT_FAILURE;
    }
       
    sort.index= malloc(sizeof(long int) *NUMOFENTRIES);
    assert(sort.index!=NULL);
    FILE *flong=fopen(argv[2],"r");
       
    if (flong==NULL) {
        fprintf(stderr,"cant find the file");
        return EXIT_FAILURE;
    }
    
    while( fgetc(flong) != EOF){
        
        while(fgets(row, 80, flong)){
       
            sscanf(row,"%ld", &sort.index[idx]);
            idx++;
      }
    }
    
         if(strcmp(argv[1],a)==0){
            qsort(sort.index,NUMOFENTRIES,sizeof(long int),compareIntervals_auf);
          }
         if(strcmp(argv[1],d)==0){
            qsort(sort.index,NUMOFENTRIES,sizeof(long int),compareIntervals_ab);
          }
    
        array_show(sort.index,NUMOFENTRIES);
        fclose(flong);
        free(sort.index);
        return 0;
    }

