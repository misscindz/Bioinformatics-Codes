//Aufgabe 1.3
//Gruppe: Achoukhi, Wang, Ching

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
    
    int year;    
    
    if ((argc < 2) || (sscanf(argv[1],"%d", &year)==0)){
        fprintf(stderr,"Usage: [program] [year]\nMake sure that year is a number\n");
        return EXIT_FAILURE;
    }
    
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)){
        printf("%d is a leap-year\n", year);
    }
    else{
        printf("%d is not a leap-year\n", year);
    }
    return EXIT_SUCCESS;
    
}