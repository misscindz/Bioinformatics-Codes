//GIK Blatt 6
//Aufgabe 1
//Gruppe: Ching, Achoukhi, Breitschuh

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#include "core/unused_api.h"
#include "gt_suffixtree.h"
#include "stree-mum.h"

typedef struct ProcessData{
	GtUword minlength;
	bool withsequence;
}ProcessData; 

bool checkLeftMaximality(const GtStree *stree,
                         const GtUword left,
                         const GtUword right){
  
  if(left == 0){
    return true;
  }
  if(gt_stree_encoded_char(stree,left-1) != 
                    (gt_stree_encoded_char(stree,right-1))){
    return true;
  }
  return false;
}

int findMums(void *process_data,
             const GtStree *stree,
             const GtStreeLoc *loc){

	ProcessData *data = (ProcessData *)process_data;
	bool withsequence = data->withsequence,
                      hasleftmax = false;
  const GtUword *start_pos;
	GtUword minlength = data->minlength, 
          length = gt_stree_loc_depth(loc),
          middle = gt_stree_separator_first(stree),           
          left,
          right;

	if(length >= minlength){
    if(gt_stree_loc_leaves_count(loc) == 2){
      start_pos = gt_stree_loc_positions(stree,loc);
      left = MIN(start_pos[0],start_pos[1]);
      right = MAX(start_pos[0],start_pos[1]);

      if (left<middle && middle <right){
        hasleftmax = checkLeftMaximality(stree,left,right);

        if (hasleftmax){
          printf(GT_WU"\t"GT_WU"\t"GT_WU"\n", 
                      left,right-middle-1,length);
          if(withsequence){
            printf("%s\n", gt_stree_loc_string(stree,loc));
          }
        }
      }
    }
  }
return 0;
}

int gt_stree_mum(const char *indexname,
                 GtUword minlength,
                 bool withsequence,
                 GtError *err){
                   
  ProcessData *process_dat;
  process_dat = malloc(sizeof(*process_dat));
  process_dat->minlength = minlength;
  process_dat->withsequence = withsequence;
  
  GtStreeLocIter *gtli = gt_stree_loc_branch_iter_new(indexname, err);

  if (gtli == NULL){
    return -1;
  }

  printf("# start1\tstart2\tlength\n");
  if (gt_stree_loc_branch_iter_apply(gtli,
                                     findMums,
                                     process_dat, 
                                     err) != 0){
    return -1;  
  }

  free(process_dat); 
  gt_stree_loc_branch_iter_delete(gtli);

  return 0;
}
