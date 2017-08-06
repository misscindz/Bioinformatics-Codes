/* PBI Aufgabe 7.2
 *
 * Abgabe 29. November 2015
 * Gruppe: Wang, Achoukhi, Ching
 */
/* Der Entscheidungsbaum muss nicht immer vollstaendig durchlaufen werden,
   um die Effizienz des Programms zu erhoehen, bricht Programm bei Abbruchbedingung ab.*/
/*Abbruchsbedingung: naechstes Element ueber die gesamte Anzahl der elemente in Menge M
                     und Summe aller Elemente aus Menge M ist ungerad. */
 
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAXNOFELEMENTS 5000
#define SEEDVALUE 42349421
#define MAXVALUE 10000

typedef struct {
  int *values;
  int *isinA;
  unsigned int totalsum,nofvalues,sumA;  
} Partition;

int makepartition(Partition *part,int nextelem){
  if (part->sumA==part->totalsum/2) {
    return 0 ;
  }
  if (nextelem >= part->nofvalues || part->totalsum%2 == 1 ) {
    return -1;
  }
  part->sumA+=part->values[nextelem];                            /*set 1.element of M as true*/
  
  part->isinA[nextelem]=true;                                   /*folgen linken Kanten*/
  if (makepartition(part,nextelem+1)==0) {                       /* gucke fuer naechsten element of M (element in tiefer Ebene),ob             									sumA=totalsum/2 ist*/
                                                                  /*wenn eine Loesung die funktion (Makepartition) gefunden hat, retrun 0;*/  
    return 0;
  }
  part->isinA[nextelem]=false;                                  /*wiederholen all fuer element auf linken seiten()*/
                                                                 /*folgen rechten kanten*/
  part->sumA-=part->values[nextelem];
 
  if (makepartition(part,nextelem+1)==0) {    
    return 0;
  }
  return -1;
}


int main(int argc, char*argv[]){
  int *isinB;
  unsigned int j=0,i=0,n;
  srand48(SEEDVALUE);
  Partition part;
 
  while(j<=argc-1){
    sscanf(argv[j],"%d",&n); // Warum in einer Schleife wenn nur ein Parameter erlaubt ist? 
    j++;		     // Die Schleife würde außerdem die selbe Variable ständig überschreiben
  }
 
  if(argc!=2){   // diese Überprüfung immer als erstes 
    fprintf(stderr,"no input number\n"); // oder zu viele, auf jeden fall nicht genau eine
    fprintf(stderr,"Usage:%s the maximal positive number allowed is 5000\n",argv[0]);
    return EXIT_FAILURE;
  }
  else{
    if (i<MAXNOFELEMENTS) { // Und wenn nicht? Der Fall n > MAXNOFELEMENTS wird nicht abegfangen und führt zum Absturz!
      part.sumA=0;
      part.totalsum=0;
      part.nofvalues=n;
      part.values=malloc(sizeof(int) *part.nofvalues);             /*stellt speicherplatz fuer array values and isinan ein*/
      part.isinA=malloc(sizeof(int) *part.nofvalues);
      isinB=malloc(sizeof(int) *part.nofvalues); // Diese Variable ist nicht nötig
    }
    for (i=0; i<part.nofvalues; i++) {
      part.values[i] = (drand48() * (double)(MAXVALUE-1))+1;
      printf("%d\n", part.values[i]);
      part.isinA[i] = false;  // false ist ein bool, euer Array isinA ist aber ein int Array     /*summe der elemente in M berechnen*/
      part.totalsum += part.values[i];
    }
    if(makepartition(&part,0)==0){                              /*durchsuchen alle Loesung, die von der funktion(makepartition) gemacht wurden*/
      printf("Die Zahlenmenge der Groessen %d kann folgendermassen in zwei summengleiche Untermenge mit der Summe %d aufgeteilt werden:\n",n,part.sumA);
      printf("Zahlen in Untermenge A:\n");
      
      for(i=0;i<part.nofvalues;i++){                           /*wenn es true ist, d.h element zu array A gehoert*/
        if (part.isinA[i]==true) {
          part.isinA[i]=part.values[i]; // warum wird dieser Wert jetzt in dem isinA Array gespeichert>
          printf("%d\n", part.isinA[i]);
        }
      }
      printf("Zahlen in Untermenge B:");
      for (i=0; i<part.nofvalues;i++) { 
        if (part.isinA[i]==false) {    // gefährlich, da wir die Werte oben geändert haben.                  /*wenn es false ist, d.h element zu array B gehoert*/
          isinB[i]=part.values[i];
          printf("%d\n",isinB[i]); // s.o./
        }
      }
    }
    else{
      printf("Die Zahlenmenge %d der Groesse %d kann nicht in zwei summengleiche Untermengen aufgeteilt werden!", part.values[i],n);
      // Diese Ausgabe macht leider nicht so viel Sinn, warum wird hier part.values[i] ausgegeben?
    }
     
  }
  return EXIT_SUCCESS;
}
 

