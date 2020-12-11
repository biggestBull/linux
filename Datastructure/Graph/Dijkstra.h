
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include"./ConstructLJB.h"
#include"../../data_structure/Heap/Heap.h"

#define _INT_MAX (unsigned int)(-1)/2

graphinfo_st * 
dijkstra(ljb_st ** ljbs,int size,int v);

#endif
