//Aufgabe 1.4
//Gruppe: Achoukhi, Wang, Ching

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
    
    int i;
    
    for (i=0; i<=100; i++){
        printf("%3.1f Celsius are %3.1f Fahrenheit\n", (double)i, (double)i*1.8 + 32 );
    }    
    
    return EXIT_SUCCESS;
    
}