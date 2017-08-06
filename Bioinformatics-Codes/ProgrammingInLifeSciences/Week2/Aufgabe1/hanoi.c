#include <stdio.h>
#include <stdlib.h>
 
// C recursive function to solve tower of hanoi puzzle
void hanoimoves (int n, int rod1, int rod2, int rod3)
{
    if (n == 1)
    {
        printf("(%d,%d) ", rod1, rod2);
        return;
    }
    hanoimoves(n-1, rod1, rod3, rod2);
    printf("(%d,%d) ", rod1, rod2);
    hanoimoves(n-1, rod3, rod2, rod1);
}
 
int main(int argc, char *argv[])
{
    int n;
    if (!(sscanf(argv[1],"%d", &n))){
        fprintf(stderr,"input should be a number\n");
        return EXIT_FAILURE;
    }; 
    
    printf("\n");
    hanoimoves(n, 1, 2, 3);
    printf("\n");
    return EXIT_SUCCESS;
}