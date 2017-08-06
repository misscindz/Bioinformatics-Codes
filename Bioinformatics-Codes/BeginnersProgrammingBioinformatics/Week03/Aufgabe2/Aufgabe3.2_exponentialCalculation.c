#include <stdlib.h>
#include <stdio.h>


int exp_calc(int unsigned base, int unsigned exp)
{
  int i, result;

  if(base==0||exp==0){
  fprintf(stderr,"wrong input\n");
  return EXIT_FAILURE;
  }
  else{
    result=base;
    for(i=1; i<exp; i++){
        result*=base;
    }
  return (printf("Result: %d", result));
  }
}



int main ()
{
  int base, exp;

  printf("input base\n");
  scanf("%d",&base);

  printf("input exponent\n");
  scanf("%d",&exp);

  exp_calc(base,exp);
  return 1;
}

/*
 * Globale Variablen wurden entfernt. Es wurden Variablen gestrichen und "bessere" Variablennamen vergeben,
 * die eindeutig zeigen, was fuer eine Aufgabe diese haben.
 * Da die 2 Funktionen ask und do_it zu Unuebersichtlichkeit gefuehrt haben, wurden diese entfernt.
 * Stattdessen wurde eine neue Funktion exp_calc erstellt. Die Fuehrt die Exponentialberechnung durch.
 * Die Scan-Abfrage der Variablen befinden sich nun in der main-Funktion und uebergibt die Werte exp und base als Parameter an die exp_calc Funktion weiter.
 * Die Ueberpruefung der Werte auf Wahr- oder Falschheit findet nun in der neuen exp_calc Funktion statt.
 * Zuvor war diese Ueberprufung gestueckelt und sehr unuebersichtlich ueber 3 Funktionen verteilt.
 * Die Variablen exp und base sind unsigned, damit nur positive Zahlen erlaubt sind.
 * Dies ermoegtlicht eine uebersichtlichere Abfrage nach Wahr oder Falschheit der werte durch if(base==0||exp==0)-->fehler anstatt if a =! 1 || a<=0 (usw...)
 * Nur alle Werte exp > 0 und base >0 sind in diesem Programm erlaubt (haben die Bedingung vom Original-Programm uebernommen).
 * Zudem waren die return-Werte der Funktion bei False oder True nicht 0 oder 1, so wie es ueblicherweise sein sollte.
 */
