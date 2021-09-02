#ifndef AVL_HEAD
#define AVL_HEAD

#include<stdio.h>
#include<stdlib.h>

typedef unsigned long ul_ptr_t;

enum Value_Type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct AVLTree_Node{
  int key;
  void * value;
  enum Value_Type value_type;
  int height;
  struct AVLTree_Node * left;
  struct AVLTree_Node * right;
};

struct AVLTree_Node *
create_AVLT(int key,void * value,enum Value_Type value_type);

int
search_AVLT(struct AVLTree_Node * Root,int key,void * rt_value);

struct AVLTree_Node *
insert_AVLT(struct AVLTree_Node * Root,int key,void * value);

struct AVLTree_Node *
delete_AVLT(struct AVLTree_Node * Root,int key);

#endif
