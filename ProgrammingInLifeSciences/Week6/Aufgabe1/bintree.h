#ifndef BINTREE_H
#define BINTREE_H
#include <stdbool.h>



/* Funktion used by <GenBinTree> to handle duplicate entries. First parameter
   <old> will be the value stored in the <GenBinTree>, second <new> will be the
   value to add. */
typedef void (*Combinefunction)(void *old, const void *new);


/* Funktion used by <GenBinTree> to compare two objects. Return values should be
   as follows
   < 0 if first value is smaller than second
   > 0 if first value is larger than second
   0 if the values are considered equal */
typedef int (*Cmpfunction)(const void *, const void *);


/* Funktion used by <GenBinTree> to free objects stored within a node */
typedef void (*Freefunction)(void *);


/* Funktion called for every object stored within the nodes of a <GenBinTree>
   when gbt_enumvalues is called */
typedef void (*Applyfunction)(const void *, void *);


typedef struct BinTreeNode BinTreeNode;

struct BinTreeNode {
  void *value;
  BinTreeNode *left,
	      *right;
};


/* The general Binary Tree class, used to store arbitrary objects in a sorted
   (log(n) access) manner. */
typedef struct GenBinTree GenBinTree;

struct GenBinTree {
  BinTreeNode *t_root;
  Cmpfunction cmp_node_value;            /* pointer to compare function, returns
				            < 0 if value1 < value2,
                                            0 if value1==value2 * and
                                            > 0 if value1 > value2 */
  Combinefunction combine_node_value;    /* pointer to function that handles
                                            duplicate values */
  Freefunction free_node_value;          /* pointer to Funktion that frees the contents
				            of a node pointed to by value */
};




typedef struct
{
  unsigned long count;
  char *word;
} WordNodeInfo;



/* Return a pointer to a new <GenBinTree> object. <cmp_node_value> will be used
   to compare and sort the values to be stored in the <GenBinTree>,
   <combine_node_value> is called if <cmp_node_value> indicates a duplicate
   value during a gbt_add()-call, <free_value> will be called for every value in
   the <GenBinTree> if it is deleted. */
GenBinTree *gbt_new(Cmpfunction cmp_node_value, Combinefunction combine_node_value, Freefunction free_node_value);


/* add a new element to binary tree, uses cmp_node_value. Return 
   true if node corresponding to value was added. Otherwise 
   combine_node_value function was called and the gtb_add returns false */
bool gbt_add(GenBinTree *bintree,void *new_value);


/* free the memory of the whole tree, also of the values inside the nodes,
 * uses free_node_value */
void gbt_delete(GenBinTree *bintree);


/* iterate in sorted order over the values stored in the binary tree.
   Apply the function apply_node_value to the values. data is supplied
   as second argument to this function. */
void gbt_enumvalues(const GenBinTree *bintree, Applyfunction apply_node_value, void *data);

void combine_node(void *node_value, __attribute__ ((unused)) const void *new_value);

#endif

