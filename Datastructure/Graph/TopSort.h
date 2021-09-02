#ifndef TOPSORT_H
#define TOPSORT_H

#include"./ConstructLJB.h"
#include"../../data_structure/Queue/queue.h"

static int 
_initQueue(ljb_st ** ljbs,struct Queue * queue,int * indegree,int size);
int *
topSort(ljb_st ** ljbs,int * indegree,int size);


#endif
