// Aufgabe 1 Gruppe 5 Achoukhi-Breitschuh-Ching-Moradi
/*
  Copyright (c) 2016 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2016 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "scorematrix.h"
#include "swalign.h"


/* The following function computes an optimal local alignment given two
   sequences and a score function. Delete
   __attribute__ ((unused)) 
   once the code using these parameters is written.
*/


void smithwaterman_algorithm(OptimalLocalAlignmentCoords *coords,
                             __attribute__ ((unused)) const Scorematrix *scorematrix,
                             __attribute__ ((unused)) Score insertionscore,
                             __attribute__ ((unused)) Score deletionscore,
                             __attribute__ ((unused)) const unsigned char *useq,
                             __attribute__ ((unused)) unsigned long ulen,
                             __attribute__ ((unused)) const unsigned char *vseq,
                             __attribute__ ((unused)) unsigned long vlen)
{

    /* this needs to be implemented. Here we just assign 0 */
    coords->ustart = 0;
    coords->usubstringlength = 0;
    coords->vstart = 0;
    coords->vsubstringlength = 0;
    coords->score = 0;

    int row          = 0;
    int column       = 0;
    Score curr_value = 0;
    Score max_value  = 0;
    int row_max      = 0;
    int column_max   = 0;

    int d_value = 0;//deletion
    int r_value = 0;//replacement
    int i_value = 0;//insertion
    
    int trace_back_value = 0;//insertion
    int start_found = 0;
    int len_found = 0;

        
    for(row=0; row < ulen; ++row)
    {
        for(column=0; column < vlen; ++column)
        {
            if((useq[row] == vseq[column]) && (start_found == 0))
            {
                coords->ustart = row;
                coords->vstart = column;
                start_found = 1;
            }

            curr_value = scorematrix_score_get(scorematrix, useq[row], vseq[column]);
            if((curr_value >= max_value) && (useq[row] == vseq[column]))
            {
                if((curr_value == max_value) && (len_found != row))
                {
                    coords->usubstringlength = coords->usubstringlength + 1;
                    coords->vsubstringlength = coords->vsubstringlength + 1;
                    len_found = row;
                }                

                if(curr_value == max_value)
                {
                    coords->score = coords->score + 1;
                }
                max_value  = curr_value;      
                row_max    = row;
                column_max = column;
            }           
        }
    }

    trace_back_value = scorematrix_score_get(scorematrix, useq[row_max], vseq[column_max]);

    while(trace_back_value != 0)
    {       
        if((column_max == 0) || (row_max == 0))
        {
            break;
            if((column_max == 0) && (row_max == 0))
            {
                break;
            }
            if(column_max == 0)
            {
                row_max = row_max - 1;
                trace_back_value = scorematrix_score_get(scorematrix, useq[row_max], vseq[column_max]);
            }
            else
            {
                column_max = column_max - 1;
                trace_back_value = scorematrix_score_get(scorematrix, useq[row_max], vseq[column_max]);                
            }
        }
        else
        {
            d_value = scorematrix_score_get(scorematrix, useq[row_max], vseq[column_max-1]);
            i_value = scorematrix_score_get(scorematrix, useq[row_max-1], vseq[column_max]);
            r_value = scorematrix_score_get(scorematrix, useq[row_max-1], vseq[column_max-1]);        
 
            //Priority case
            if(   (d_value == i_value)
               ||
                  (r_value == i_value)
               ||
                  (r_value == d_value)
              )
            {                
                if(  
                    ((r_value == d_value) && (d_value >= i_value))    //replacement
                  ||
                    ((r_value == i_value) && (r_value >= d_value))    //insertion
                  )
                {                    
                    trace_back_value = r_value;
                    column_max = column_max - 1;
                    row_max = row_max - 1;
                }
                else if((d_value == i_value) && (d_value >= r_value)) //deletion
                {
                    trace_back_value = d_value;  
                    column_max = column_max - 1;
                }
                else
                {
                    trace_back_value = r_value;
                    column_max = column_max - 1;
                    row_max = row_max - 1;
                }
            }
            else
            {
                if((d_value > i_value) && (d_value > r_value))
                {
                    trace_back_value = d_value;
                    column_max = column_max - 1;
                }
                else if ((r_value > i_value) && (r_value > d_value))
                {
                    trace_back_value = r_value;
                    column_max = column_max - 1;
                    row_max = row_max - 1;                    
                }
                else
                {
                    trace_back_value = i_value;
                    row_max = row_max - 1;
                }
            }               
        }                 
    }
}
