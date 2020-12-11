#ifndef SKIPTABLE_H
#define SKIPTABLE_H

#include<stdlib.h>
#include<time.h>
#include"../Stack/stack.h"

struct ST_Node{
   int key;
   void * value;
   struct ST_Node * down,*next;
};

struct _ST{
  struct ST_Node * head;
  struct Stack stack;
  int layer_range;
};

struct _ST *
createST(int layer_num);
int
insertST(struct _ST * st,int key,void * value);
int
deleteST(struct _ST * st,int key);
int 
searchST(struct _ST * st,int key,void * rt_value);

#endif
