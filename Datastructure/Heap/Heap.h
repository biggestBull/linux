#ifndef HEAP_HEAD
#define HEAP_HEAD

#include"../type.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Heap{
   enum _TYPE type;
   int size;
   int cur_size;
   int flag;                           //non-0max or 0min
   void * (*func)(void *);
   unsigned long * arr;
};

struct Heap *
create_Heap(enum _TYPE heap_type,int flag,void * (*func)(void *));
void
release_heap(struct Heap * heap);
int
construct_Heap_fromArray(struct Heap * heap,void * arr,int arr_len);
heap_insert(struct Heap * heap,void * value);
int
heap_pop(struct Heap * heap,void * rt_value);

#endif
