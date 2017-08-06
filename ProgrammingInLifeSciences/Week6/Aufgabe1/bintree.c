/* Wang Ching Achoukhi */
/* Aufgabe 1 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bintree.h"
#include "tokenizer.h"


void combine_node(void *node_value,
                             __attribute__ ((unused)) /*@unused@*/
                             const void *new_value)
{
  WordNodeInfo *word_node_value = (WordNodeInfo*)node_value;
  word_node_value->count = word_node_value->count + 1;
  node_value = (void*)word_node_value->count;
}
 
/* Return a pointer to a new <GenBinTree> object. <cmp_node_value> will be used
   to compare and sort the values to be stored in the <GenBinTree>,
   <combine_node_value> is called if <cmp_node_value> indicates a duplicate
   value during a gbt_add()-call, <free_value> will be called for every value in
   the <GenBinTree> if it is deleted. */
GenBinTree *gbt_new(Cmpfunction cmp_node_value, Combinefunction combine_node_value, Freefunction free_node_value)
{
  GenBinTree *gbt_ptr = NULL;

  gbt_ptr = malloc(sizeof(GenBinTree));
  assert(gbt_ptr != NULL);
  gbt_ptr->cmp_node_value     = cmp_node_value;
  gbt_ptr->combine_node_value = combine_node_value;
  gbt_ptr->free_node_value    = free_node_value;
  gbt_ptr->t_root             = NULL;
  
  return gbt_ptr;
}


BinTreeNode* insert_node(BinTreeNode* node_ptr, GenBinTree *bintree, void *new_value)
{
  BinTreeNode *new_node_ptr = NULL;

  if(node_ptr == NULL)
  {
    new_node_ptr = (BinTreeNode*)malloc(sizeof(BinTreeNode));
    assert(new_node_ptr != NULL);
    new_node_ptr->left  = NULL;
    new_node_ptr->right = NULL;
    new_node_ptr->value = new_value;   
  }
  else
  {   
    if((bintree->cmp_node_value(node_ptr->value, new_value)) == 0 )
    {
      combine_node(node_ptr->value, new_value);
      return NULL;
    }
    else
    {
      if((bintree->cmp_node_value(node_ptr->value, new_value)) > 0)
      {
        new_node_ptr = insert_node(node_ptr->left, bintree, new_value);
        if((node_ptr->left == NULL) && (new_node_ptr != NULL))
        {
          node_ptr->left = new_node_ptr;
        }
      }
      else
      {
        new_node_ptr = insert_node(node_ptr->right, bintree, new_value);
        if((node_ptr->right == NULL) && (new_node_ptr != NULL))
        {
          node_ptr->right = new_node_ptr; 
        }
      }
    }
  }
  return new_node_ptr;
} 


/* add a new element to binary tree, uses cmp_node_value. Return 
   true if node corresponding to value was added. Otherwise 
   combine_node_value function was called and the gtb_add returns false */
bool gbt_add(GenBinTree *bintree,void *new_value)
{
  bool success = true;
  BinTreeNode* new_added_node = NULL;
 
  new_added_node = insert_node(bintree->t_root, bintree, new_value);

  if((bintree->t_root == NULL) && (new_added_node != NULL))
  {
    bintree->t_root = new_added_node;
  }
  if(new_added_node == NULL)
  {
    success = false;
  }
  return success;
}


void remove_node(GenBinTree *bintree, BinTreeNode *node)
{ 
  if(node != NULL)
  {
    remove_node(bintree, node->left);
    remove_node(bintree, node->right);
    bintree->free_node_value(node->value);
    free(node);
  }
}


/* free the memory of the whole tree, also of the values inside the nodes,
 * uses free_node_value */
void gbt_delete(GenBinTree *bintree)
{
  assert(bintree != NULL);
  remove_node(bintree, bintree->t_root);
  free(bintree);
}

void iterate_in_tree(BinTreeNode *node, Applyfunction apply_node_value, void *data)
{
  if(node != NULL)
  {
    iterate_in_tree(node->right, apply_node_value, data);
    apply_node_value(node->value, data);
    iterate_in_tree(node->left, apply_node_value, data);
  }
}

/* iterate in sorted order over the values stored in the binary tree.
   Apply the function apply_node_value to the values. data is supplied
   as second argument to this function. */
void gbt_enumvalues(const GenBinTree *bintree, Applyfunction apply_node_value, void *data)
{
  iterate_in_tree(bintree->t_root, apply_node_value, data);
}


