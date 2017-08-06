/* PBI Aufgabe 8.1 
 *
 * Abgabe 7. Deczember 2015
 * Gruppe: Wang, Achoukhi, Ching
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct List_element{
    unsigned long value;
    struct List_element *prev;
    struct List_element *next;
}List_element;

typedef struct {
    List_element *first_element;
    List_element *last_element;
}List;

List *list_new(void){
    List *list = (List*)calloc(2,sizeof(List_element));
    assert(list != NULL);
    return list;
}
  
unsigned long list_element_get_value (const List_element *element){ 
    return element->value;  
}

List_element *list_first (const List * list){;
    if(list->first_element==NULL){
      return NULL;
    }
    else{
      return list->first_element; 
    }
}

List_element *list_last (const List* list){    
    assert(list->last_element!=NULL);
    if (list->last_element==NULL){       
      return NULL;
    }
    else {
      return list->last_element;
    }
}   

List_element *list_element_next (const List_element *element){
   assert(element !=NULL); 
   if (element->next ==  NULL){
      return NULL;
    }
    else{
      return element->next;
    }
}

List_element* list_element_prev (const List_element* element){
    assert(element !=NULL); 
    if (element->prev ==  NULL){
      return NULL;
    }
    else{
      return element->prev;
    }
}

void list_append(List* list, unsigned long value){
    assert(list != NULL);
    List_element* new_element = (List_element*)malloc(sizeof(List_element));
    new_element->value = value;
    new_element->next = NULL; 
    //Erstes Element wird hinzugefuegt
    if(list->first_element== NULL){
      list->first_element= new_element;
      list->first_element->prev= NULL;
    }
    //zweites Element wird hinzugefuegt und mit erstem verknuepft
    else if((list->first_element != NULL) &&(list->last_element==NULL)){	
      list->first_element->next = new_element;	
      new_element->prev = list->first_element;   
      list->last_element=new_element;
    }
    //restliche Elemente
    else{
      list->last_element->next=new_element;
      new_element->prev = list->last_element;
      list->last_element = new_element;
    }
    //free(new_element); ---> Valgrind "Invalid free() / delete / delete[] / realloc()"
}

void list_show (const List *list){   
    unsigned long idx=0;
    List_element* temp;
    temp = list->first_element;
    assert(temp != NULL);
    printf("Accessing list\n---------------\n");
    while (temp != 0){
      printf("Index: %2lu \t Wert: %lu\n", idx++,temp->value);
      temp=temp->next; 
    }
    free(temp);

}

void list_element_show (const List_element *element){
    if (element == NULL){ //Bei Zugriff auf nicht vorhandem Element
      printf("Element you are trying to access does not exist!\n");
    }
    else{
      printf("%lu\n",list_element_get_value(element));
    }
}

void list_delete_element (List *list, List_element *element){
    assert (list != NULL);
    assert (element != NULL);
    List_element *previous_element=list_element_prev(element);
    List_element *next_element = list_element_next(element);  
    if (element->prev == NULL){ // Wenn erstes element geloescht werden soll
      next_element->prev=NULL;
      list->first_element=next_element;      
    }   
    else if (element->next == NULL){ //wenn letztes Element geloescht werden soll
      previous_element->next=NULL;
      list->last_element=previous_element;   
    }
    else{
      previous_element->next = next_element;
      next_element->prev = previous_element;
    }
}

void list_delete (List *list){
    List_element* temp;
    temp = list->first_element;
    while(temp != list->last_element){
      temp=temp->next;
      free(temp->prev);
    }
    free(list);
    free(temp);
}

/*
 * 
 * 
Valgrind output + weitere Anmerkungen dazu siehe unten:
------------------------------------------------------------------------------------------------------------------------------------

cindy@cindy-Lenovo-IdeaPad-Yoga-13:~/Documents/Studium/PBI/Blatt8/final$ valgrind --leak-check=yes ./linkedlist_test.x linkedlist.dat
==4950== Memcheck, a memory error detector
==4950== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==4950== Using Valgrind-3.10.0.SVN and LibVEX; rerun with -h for copyright info
==4950== Command: ./linkedlist_test.x linkedlist.dat
==4950== 
Accessing list
---------------
Index:  0 	 Wert: 3
Index:  1 	 Wert: 234
Index:  2 	 Wert: 1
Index:  3 	 Wert: 12343
Index:  4 	 Wert: 145
Index:  5 	 Wert: 15
Index:  6 	 Wert: 8714
Index:  7 	 Wert: 1274987
Index:  8 	 Wert: 32942
Index:  9 	 Wert: 329491
Index: 10 	 Wert: 12
Index: 11 	 Wert: 2
Index: 12 	 Wert: 381247
Index: 13 	 Wert: 3
Index: 14 	 Wert: 230
Index: 15 	 Wert: 4
Index: 16 	 Wert: 24374
Index: 17 	 Wert: 2192391
Index: 18 	 Wert: 412
Index: 19 	 Wert: 4131
Index: 20 	 Wert: 12371
Index: 21 	 Wert: 12
Index: 22 	 Wert: 124701
Index: 23 	 Wert: 2947351
Index: 24 	 Wert: 17238
Index: 25 	 Wert: 2342
Index: 26 	 Wert: 12
Index: 27 	 Wert: 3
Index: 28 	 Wert: 4
Index: 29 	 Wert: 10234
Index: 30 	 Wert: 106734287
First element: 3
New first element: 234
Element you are trying to access does not exist!
10234
3
==4950== 
==4950== HEAP SUMMARY:
==4950==     in use at exit: 24 bytes in 1 blocks
==4950==   total heap usage: 34 allocs, 33 frees, 1,384 bytes allocated
==4950== 
==4950== 24 bytes in 1 blocks are definitely lost in loss record 1 of 1
==4950==    at 0x4C2AB80: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==4950==    by 0x400C5B: list_append (linkedlist.c:74)
==4950==    by 0x4009BC: main (linkedlist_test.c:26)
==4950== 
==4950== LEAK SUMMARY:
==4950==    definitely lost: 24 bytes in 1 blocks
==4950==    indirectly lost: 0 bytes in 0 blocks
==4950==      possibly lost: 0 bytes in 0 blocks
==4950==    still reachable: 0 bytes in 0 blocks
==4950==         suppressed: 0 bytes in 0 blocks
==4950== 
==4950== For counts of detected and suppressed errors, rerun with: -v
==4950== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

--------------------------------------------------------------------------------------------------------------------------------------------------
Anmerkung: der Versuch, den letzten memory leak in Funktion "list_append" zu beheben scheiterte leider. Mit free(new_element), 
zeigt Valgrind keine leaks mehr an, dafuer aber "Invalid free() / delete / delete[] / realloc()"-Fehler

*/
