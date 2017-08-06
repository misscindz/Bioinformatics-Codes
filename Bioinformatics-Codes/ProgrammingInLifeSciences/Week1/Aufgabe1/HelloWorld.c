//Aufgabe 1.1
//Gruppe: Achoukhi, Wang, Ching

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if (argc > 2){
        fprintf(stderr,"Too many arguments entered.\n");
        return EXIT_FAILURE;
    }
    else if (argc < 2){
        printf("Hello World\n");
        return EXIT_SUCCESS;
    }
    else{
        printf("Hello %s\n", argv[1]);
        return EXIT_SUCCESS;
    }
}