/* Aufgabe 4.2, Gruppe 5: Achoukhi, Breitschuh, Ching, Moradi */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
  char *p, *t;
  unsigned int chars[256];
  unsigned int i = 0, j, k, match;
  
  if(argc != 3) {
    fprintf(stderr, "USAGE: %s <text> <pattern>\n", argv[0]);
    return EXIT_FAILURE;
  }
  t = argv[1];
  p = argv[2];
  k = strlen(p);
  
  for(j = 0; j < 256; ++j) {
    chars[j] = k;
  }
  for(j = 0; j < k-1; ++j) {
    chars[(int)p[j]] = k-j-1; /*AS: sehr schön*/
  }
  
  while(i < strlen(t)) { /*AS: problematisch ab i=strlen(t)-k+1*/
    match = 0;
    for(j = 0; j < k; ++j) {
      if(p[j] == t[i+j]) {/* AS: Achtung in Verbindung mit der while-Schleife,
                           * im Fall i=strlen-1, ab j>0 betrachtet ihr mit i+j
                           * Zeichen außerhalb von t*/
        match++;
      } else {
       break; 
      }
    }
    if(match == k) {
      printf("Match at position %d.\n", i);
      return EXIT_SUCCESS; /* AS: warum beendet ihr das Programm sobald ein
                            * match gefunden wurde? */
    }
    i += chars[(int)t[i+k-1]];
  }
  printf("No match was found.\n");
  
  return EXIT_SUCCESS;
}

/* AS: 2 Punkte. Euer Programm liefert lediglich eine Matchposition zurück und
 * wird beendet sobald diese gefunden wird. Ziel der Aufgabe war es aber alle
 * Positionen zu finden, wo ein Match von p auf t beginnt. Idee ist sonst gut.
 * Kleine Umsetzungsfehler in den Schleifen. Ihr lauft über t hinaus,
 * funktioniert an dieser Stelle weil bei der if-Abfrage im Vergleich mit '/0'
 * immer der else-Fall eintritt, sodass es nicht zu einem Speicherzugriffsfehler
 * kommt. Faktisch laufen die Schleifen aber nicht ganz richtig */
