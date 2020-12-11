#ifndef BIN_TREE_HEADER
#define BIN_TREE_HEADER

#include<stdio.h>
#include<stdlib.h>

enum Value_type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct BinTree_Node{
   int key;
   enum Value_type value_type;
   void * value;
   struct BinTree_Node * left;
   struct BinTree_Node * right;
   short flag;
};

typedef unsigned long ul_ptr_t;

struct BinTree_Node * create_BinTree(int key,ul_ptr_t value,enum Value_type value_type);
int search_BinTree(struct BinTree_Node * Root,int key,void *  rt_value);
int insert_BinTree(struct BinTree_Node * Root,int key,ul_ptr_t value);
int delete_BinTree(struct BinTree_Node * Root,int key);
#endif
