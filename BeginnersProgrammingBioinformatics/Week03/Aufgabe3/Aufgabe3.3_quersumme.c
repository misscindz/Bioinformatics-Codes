#include<stdio.h>



int quersumme (int x)
{
  
    if (x < 10)
    {
        printf ("%d\n", x);
        return (x);

    }
    else 
    {
        
        return ((x%10) + quersumme(x/10));
    }
}


int main (int argc, char* argv[])
{ 
    int x;
    int erg;

    if (argc != 2)
    {
        printf ("Fehler. Bitte Zahl in der Kommandozeile eingeben");
    }
    else 
    {
        printf ("Sie haben eingegeben: %s\n", argv[0]);
        x = atoi (argv[1]);
    }
        erg = quersumme(x);
        printf ("Die Quersumme Ihrer Zahl lautet:  %d\n", erg);

    return 0;
}




