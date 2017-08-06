#define NUMOFBITS 32  /* number of bits in bitvector */
#include<stdio.h>

void decimal2bitvector (char *bitvector, unsigned int n)
{
 int i=0;
 int erg = 0;

    for (i=0; i<32; i++ )
    { 
        erg = n % 2;
        if(erg == 1)
        {
            *(bitvector + 31 - i) = '1';
        }
        else
        {
            *(bitvector + 31 - i) = '0';
        }

        n= n/2;
    }
    *(bitvector + 32) = '\0';
}

unsigned int bitvector2decimal (const char* bitvector)
{    
    int erg = 0;
    int help = 1;
    int i;

    for (i=0; i<32; i++)
    {
        if ( *(bitvector + 31 - i) == '1' ) 
        {
           erg = erg + help;
        }
      
        help = help * 2;
       
    }

    printf ("%d\n", erg);

    return erg;
}



int main () 
{
    int wahl;
    
    printf ("Für Dezimal in Binaer geben Sie '1' ein. Für Binaer in Dezimal geben Sie '2'ein\n");
    scanf ("%d", &wahl);
    if (wahl == 1)
    {  
        int x;
        char bitfeld [33];
     
        printf ("Bitte geben Sie eine Zahl ein:  ");
        scanf ("%d", &x);   
        decimal2bitvector (&bitfeld[0], x);
        printf("%s", bitfeld);
    }
    
    else if (wahl == 2)
    {
        char array [33];
        
        printf ("Bitte geben Sie eine Binaere Zahl mit 32 Zeichen ein:  ");
        scanf ("%s\n", array);
        printf ("Sie haben eingegeben: %s\n", array);
        bitvector2decimal (&array[0]);
        
    }

  return 0;
}




