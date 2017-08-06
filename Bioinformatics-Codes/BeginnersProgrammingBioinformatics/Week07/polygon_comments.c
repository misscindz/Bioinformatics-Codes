/*Aufgabe7.1 Wang_Ching_Achoukhi */
/*         29.11.15              */ 


#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define NEW_LINE        10 


typedef struct 
{
  double x;
  double y;
} Point;

typedef struct
{
 Point *pt_Point; // Name nicht gut gewählt, wenigstns sollte hier die Mehazahl (points) verwendet werden. 
 int count_point;   // count könnte unsigned gesetzt werden, da ein negativer Count nicht zulässig ist. 
}Polygon;

/* Die Funktion berechnet den Abstand zwischen zwei Punkten */
double calc_dist_point(Point p1, Point p2) // Warum anders benannt als in der Aufgabenstellung? 
{
  return sqrt( (pow((p1.x-p2.x), 2)) + (pow ((p1.y-p2.y), 2)) ); // Ein paar Klammern (jeweils um die pow Funktion) könnte man weglassen. 
}

/* Die Funktion fuegt ein neues Element (Punkt) in die dynamisch erzeugte Struktur hinzu. */
void polygon_add_point(Polygon *poly, const Point *p)
{  
   poly->pt_Point = realloc(poly->pt_Point, (sizeof(Point))*(poly->count_point + 1)); // Für jedes einzelne Element zu reallocieren ist ineffizient. 
   //auch nach dem realloc müsste man prüfen ob es geklappt hat oder ob die Rückgabe NULL ist. 
   *(poly->pt_Point + poly->count_point) = *p; // Bitte wenn möglich immer Array Syntax benutzen: poly->pt_Point[poly->count_point] = *p;
   /* Inkrementieren der Anzahl der Punkte nachdem ein Punkt hinzugefuegt wurde. */
   poly->count_point = poly->count_point + 1; // Bei Verwendung von ++poly->count_point oben hätte man sich diese Zeile gespart. 
}

/* Die Funktion berechnet den Umfang eines Polygons anhand der Anzahl der Punkte. */
double perimeter(const Polygon *shape)
{
  int index = 0;
  double distance = 0.0; // evtl einen Variablennamen wie perimeter nehmen

  for(index=0; index < (shape->count_point-1); index++ )
  {
    distance += calc_dist_point( *(shape->pt_Point + index), *(shape->pt_Point + index + 1)); // auch hier Array Syntax verwenden: calc_dist_point(shape->pt_Point[index], shape->pt_Point[index+1]);
  }
  distance += calc_dist_point( *(shape->pt_Point), *(shape->pt_Point + index)); // s.o.

  return distance;
}


int main ()
{ 
  FILE *input_file;

  Point read_point; 
  Polygon polyg;

  double distance;
  char cc;
  char read_line[20] = {0};
  int index = 0;
  int i;

  /* Dynamische Allokierung des Speichers, worauf der pt_Point zeigt. */
  polyg.pt_Point = (Point*) malloc(sizeof(Point) * 1); // *1 ist unnötig. 
  polyg.count_point = 0; 

  input_file = fopen ("points.csv", "r");
  if ( input_file == NULL )
  {
    printf ("Wrong input\n"); // Error Stream verwenden. 
    return (EXIT_FAILURE) ;
  }

  while ((cc = getc(input_file)) != EOF)
  {
    if( (cc == NEW_LINE) )
    {
      /* Das Einlesen der Punkte aus den Zeilen */
      sscanf(read_line,"%lf\t%lf\n", &read_point.x, &read_point.y); // unbedingt überprüfen ob es auch klappt, ich weiß die Datei ist vorgegeben, aber es kann ja immer was sein. 
      /* Das Einfuegen des eingelesenen Punktes in die Polygonstruktur. */
      polygon_add_point(&polyg, &read_point);
      /* Zuruecksetzen des Arrayindexes fuer das Einlesen der Zeilen. */
      index = 0; 
      for(i=0; i<20; i++) read_line[i] = 0; // = ""
    }
    else
    {
      read_line[index] = cc;
      index++;
    }
  }

  fclose (input_file) ;

  /* Berechnung des Polygonumfangs. */   
  distance = perimeter(&polyg);
  free(polyg.pt_Point);

  printf("\nDer Umfang betreagt %f Einheiten\n", distance);

  return EXIT_SUCCESS;   
}


