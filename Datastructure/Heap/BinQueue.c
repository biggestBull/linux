#include<stdio.h>
#include<stdlib.h>

#define MAX_CAP 9192


enum BQ_Type{
   CHAR,
   INT,
   LONG
};

struct BQ_Forest{
   int cur_size;
   enum BQ_Type type;
   unsigned long * tree;
   void * (*func)(void *);
};


