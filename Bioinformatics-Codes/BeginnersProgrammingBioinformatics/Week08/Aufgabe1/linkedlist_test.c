#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "linkedlist.h"

int main(int argc,char *argv[])
{
  FILE *file;
  unsigned long value;
  List_element *element;
  List *list = list_new();

  if(argc != 2) {
    fprintf(stderr,"Usage: %s <filename>\n",argv[0]);
    return EXIT_FAILURE;
  }

  file = fopen(argv[1], "r");

  while (!feof(file)) {
    if(fscanf(file, "%lu ", &value) != 1) {
      fprintf(stderr, "error within file, couldn't parse\n");
      exit(EXIT_FAILURE);
    }
    /* list_ordered_insert(list, value); */
    list_append(list, value);
  }
  fclose(file);

  list_show(list);
  element = list_first(list);
  value = list_element_get_value(element);
  printf("First element: ");
  list_element_show(element);

  list_delete_element(list, element);

  printf("New first element: ");
  list_element_show(list_first(list));
  /* if (list_search(list, value)) {
    printf("Element with value %lu found.\n", value);
  }
  else {
    printf("No element with value %lu found.\n", value);
  } */

  element = list_last(list);
  list_element_show(list_element_next(element));
  list_element_show(list_element_prev(element));

  list_append(list, value);

  list_element_show(list_element_next(element));

  list_delete(list);
  return EXIT_SUCCESS;
}
