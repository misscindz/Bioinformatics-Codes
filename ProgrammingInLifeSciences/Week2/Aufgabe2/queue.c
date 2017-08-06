#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int Queueelement;

typedef struct {
    Queueelement *queuespace;
    unsigned long enqueueindex,
    dequeueindex,
    queuesize,
    no_of_elements;
}Queue;

Queue *queue_new(unsigned long queuesize){
    
    if (queuesize == 0){
        fprintf(stderr,"queuesize cannot be zero\n");
        exit (EXIT_FAILURE);
    }
    
    Queue *q = NULL;
    
    q = malloc(sizeof(Queue));
    
    if (q == NULL){
        fprintf(stderr,"memory allocation error\n");
        exit (EXIT_FAILURE);
    }

    q->queuespace = malloc(queuesize*sizeof(Queueelement));
    
    if (q->queuespace == NULL){
        fprintf(stderr,"memory allocation error\n");
        exit (EXIT_FAILURE);
    }
    
    q->queuesize = queuesize;
    q->no_of_elements = 0;
    q->enqueueindex = 1;
    q->dequeueindex = 1;
    
    return q;
    
}

bool queue_is_empty (const Queue *q){
    if (q->no_of_elements == 0){
        return true;
    }
    else{ 
        return false;
    }
}

void queue_double_size(Queue *q){
    
    int i;
    q->queuesize*=2;
    q->queuespace = realloc(q->queuespace, q->queuesize*(sizeof(Queueelement)));
    
    if (q->queuespace == NULL){
        fprintf(stderr,"memory allocation error\n");
        exit (EXIT_FAILURE);
    }
    
    for (i=0; i<q->dequeueindex; i++){
        q->queuespace[q->dequeueindex+i]=q->queuespace[i];
    }
    
    q->enqueueindex = q->dequeueindex + q->no_of_elements;  
}


void queue_enqueue(Queue *q, Queueelement elem){

    if (q->no_of_elements +1 == q->queuesize){
        queue_double_size(q);
    }
    q->queuespace[q->enqueueindex] = elem;
    q->no_of_elements++;
    q->enqueueindex = ((q->enqueueindex + 1)% q->queuesize);
}

Queueelement queue_dequeue(Queue *q){
    
    if (queue_is_empty(q)){
        fprintf(stderr,"queue is empty, dequeueing not possible\n");
        exit (EXIT_FAILURE);
    }
    Queueelement dequeue = q->queuespace[q->dequeueindex];
    q->no_of_elements--;
    q->dequeueindex = (q->dequeueindex + 1)%q->queuesize;
    return dequeue;
}

void queue_print(const Queue *q){
    int i;
    
    printf("queue= \n");
    if (q->dequeueindex < q->enqueueindex){
        for (i = (int)q->enqueueindex-1; i >= (int)q->dequeueindex; i--){
            printf("%d ",q->queuespace[i]);
        }
    }
   // else if (q->enqueueindex == 0){
    //    printf("%d ", q->queuespace[q->queuesize]);
    //}    
    else if (q->enqueueindex < q->dequeueindex){
        for (i = q->enqueueindex-1; i >= 0; i--){
            printf("%d ", q->queuespace[i]);
        }
        for (i = q->dequeueindex+(q->no_of_elements-q->enqueueindex-1); i >= q->dequeueindex; i--){
            printf("%d ", q->queuespace[i]);
        }
    }
    printf("\n");
}

void queue_delete(Queue *q){
    free(q->queuespace);
    free(q);
}
