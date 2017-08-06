/*****************************************************/
/* Header file f"ur Datenstruktur und Methoden einer */
/* doppelt verketteten Liste                         */
/*****************************************************/


/********************* Eigene Typen ******************/

#include <stdbool.h>

typedef struct List_element List_element;

/* lies value aus element */
unsigned long list_element_get_value(const List_element *element);

/* Gebe value von element aus, 'leer' wenn element=NULL */
void list_element_show(const List_element* element);

typedef struct List List;

/* Erzeuge eine neue doppelt verkettete Liste */
List* list_new(void);

/* Navigiere zum ersten Element */
List_element* list_first(const List* list);

/* Navigiere zum letzten Element */
List_element* list_last(const List* list);

/* Navigieren zum naechsten Element */
List_element* list_element_next(const List_element* element);

/* Navigieren zum vorherigen Element */
List_element* list_element_prev(const List_element* element);

/* Haengt ein neues Element an das Ende der Liste */
void list_append (List * list, unsigned long value);

/* Fuege Value als neues Element in (sortierte) Liste ein. */
void list_ordered_insert (List * list, unsigned long value);

/* Durchsuche (sortierte) Liste list nach Vorkommen von value. */
bool list_search (const List * list, unsigned long value);

/* Gebe den Inhalt der Liste aus */
void list_show (const List * list);

/* Loesche aktuelles Element aus der Liste */
void list_delete_element (List* list, List_element* element);

/* Freigabe des Speichers aller Listenelemente */
void list_delete(List* list);

