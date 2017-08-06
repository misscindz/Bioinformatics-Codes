//Aufgabe 1.2
//Gruppe: Achoukhi, Ching, Wang
// minimum value for all unsiged datatypes (i.e. char, short, int, long int) is zero.
//


#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int main()
{
    printf("The minimum value for a SIGNED CHAR = %d\n", SCHAR_MIN);
    printf("The maximum value for a SIGNED CHAR = %d\n", SCHAR_MAX);
    printf("The maximum value for an UNSIGNED CHAR = %d\n", UCHAR_MAX);
    printf("The size of the data type char = %ld\n", sizeof(char));
    printf("The size of the data type unsignedchar = %ld\n", sizeof(char));
    
    printf("The minimum value for a SHORT INT = %d\n", SHRT_MIN);
    printf("The maximum value for a SHORT INT = %d\n", SHRT_MAX);
    printf("The maximum value for an UNSIGNED SHORT INT = %d\n", USHRT_MAX);
    printf("The size of the data type short = %ld\n", sizeof(short int));
    printf("The size of the data type unsigned short = %ld\n", sizeof(unsigned short int));
    
    printf("The minimum value for an INT = %d\n", INT_MIN);
    printf("The maximum value for an INT = %d\n", INT_MAX);
    printf("The maximum value for an UNSIGNED INT = %u\n", UINT_MAX);
    printf("The size of the data type int = %ld\n", sizeof(int));
    printf("The size of the data type unsigned int = %ld\n", sizeof(unsigned int));
    
    printf("The minimum value for a LONG = %ld\n", LONG_MIN);
    printf("The maximum value for a LONG = %ld\n", LONG_MAX);
    printf("The maximum value for an UNSIGEND LONG = %lu\n", ULONG_MAX);
    printf("The size of the data type long = %ld\n", sizeof(long int));
    printf("The size of the data type unsigned long = %lu\n", sizeof(unsigned long int));
    
    return EXIT_SUCCESS;
   }