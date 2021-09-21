#ifndef SPLAYT_HEAD
#define SPLAYT_HEAD
#include<stdio.h>
#include<stdlib.h>

#define LEFT 1
#define RIGHT 2

typedef unsigned long ul_ptr_t;

enum Value_Type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct SplayTree_Node{
   int key;
   void * value;
   enum Value_Type value_type;
   int isRoot;
   struct SplayTree_Node * left;
   struct SplayTree_Node * right;
};

int
search_SplayT(struct SplayTree_Node ** Root,int key,void * rt_value);

struct SplayTree_Node *
create_SplayT(int key,void * value,enum Value_Type value_type);

int
insert_SplayT(struct SplayTree_Node * Root,int key,void * value);

int
delete_SplayT(struct SplayTree_Node * Root,int key);

#endif















