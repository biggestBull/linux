#ifndef KRUSKAL_H
#define KRUSKAL_H

#include"./DisjointSet.h"
#include"../../data_structure/Heap/Heap.h"
#include"./ConstructLJB.h"

typedef struct{
   int v1;
   int v2;
   int C;
}kruskal_st;

graphinfo_st *
kruskal(kruskal_st * ks,int size,int vnum);
#endif
