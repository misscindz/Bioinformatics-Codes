/* PBI Aufgabe 6.3
 *
 * Ching,Achoukhi,Wang
 * Abgabe: 23.11.2015
 *
 * Es gibt 168 Primzahlen, die <= 1000 sind.
 * Die letzten 5 Primzahlen <= 1000 sind 971, 977, 983, 991, 997.
 */

#define MAXNUMBER n

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void zahlen_markieren(int n, bool *marked){

    int l,i,j;

    for (l=0;l<n; l++){			    /* zunaechst fuer alle Zahlen 1 bis n den Wert 'true' speichern */
      marked[l]=true;
    }

    for (i=2; i<=n/2; i++){	     	/* laeuft bis die kleinste unmarkierte Zahl groesser als n/2 ist */
      if (marked[i-1]==true){		/* Prueft, ob eine Zahl unmarkiert ist */
        for (j=i; j<=n; j+=i){ 		/* markiert alle Vielfachen der unmarkierten Zahl (i), */
          if (j!=i){			    /* schliesst aus, dass kleinste unmarkierte Zahl als false markiert wird */
            marked[j-1]=false;
          }
        }
      }
    }
}

int main(int argc, char *argv[]){

    int n,k,primcount=0;
    bool marked[MAXNUMBER+1];

    if (argc<2){
      fprintf(stderr,"\nKein Argument eingegeben. Bitte Programm mit Zahl groesser oder gleich 2 aufrufen.\n\n");
      return EXIT_FAILURE;
    }

    else if (!(sscanf(argv[1],"%u",&n))||n<2){
      fprintf(stderr,"\nArgument ungueltig. Bitte Programm mit Zahl groesser oder gleich 2 aufrufen.\n\n");
      return EXIT_FAILURE;
    }

    else{
      sscanf(argv[1],"%u",&n);
      zahlen_markieren(n,marked); 	/*Funktion markiert alle Primzahlen als 'true', Nicht-Primzahlen als 'false' */
      printf("\nDie Primzahlen von 2 bis %d sind:\n\n",n);

      for (k=2; k<=n; k++){
        if(marked[k-1]==true){	/* gibt Primzahlen bis einschliesslich n aus und zaehlt diese in primcount */
          printf("%4d\n",k);
          primcount++;
        }
      }

      printf("\nDie Anzahl der Primzahlen von 2 bis %d betraegt: %d\n\n",n,primcount);
      return EXIT_SUCCESS;
    }
}



