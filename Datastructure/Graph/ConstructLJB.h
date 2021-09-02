

#ifndef CONST_LJB_H
#define CONST_LJB_H
#include<stdlib.h>

typedef struct _ljb_st{
   int v;
   int C;
   struct _ljb_st * next;
}ljb_st;

typedef struct {
   int known;
   int distance;
   int father;
}graphinfo_st;

ljb_st *
constructLJB(ljb_st * ljb,int v,int C);

#endif
