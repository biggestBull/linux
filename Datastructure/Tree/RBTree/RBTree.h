#ifndef RBTREE_H
#define RBTREE_H

#include<stdlib.h>

struct RBTree{
   int key;
   int color;
   void * value;
   struct RBTree * parent;
   struct RBTree * left;
   struct RBTree * right;
};
int
insertRBT(struct RBTree ** Root,int key,void * value);
int 
searchRBT(struct RBTree * Root,int key,void * rt);
int
deleteRBT(struct RBTree ** Root,int key);

#endif
