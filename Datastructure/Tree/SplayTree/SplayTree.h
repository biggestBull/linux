#ifndef SPLAYT_HEAD
#define SPLAYT_HEAD
#include<stdio.h>
#include<stdlib.h>

#define LEFT 1
#define RIGHT 2

typedef unsigned long ul_ptr_t;
typedef int key_t;

enum Value_Type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct SplayTree_Node{
   key_t key;
   void * value;
   struct SplayTree_Node * left;
   struct SplayTree_Node * right;
};

struct SplayTree_ST{
   enum Value_Type value_type;
   struct SplayTree_Node * Root;
};

struct SplayTree_ST *
create_SplayT(key_t key,enum Value_Type value_type);

void
free_SplayT(struct SplayTree_ST * SplayT);

int
search_SplayT(struct SplayTree_ST * SplayT,key_t key,void * rt_value);

int
insert_SplayT(struct SplayTree_ST * SplayT,key_t key,void * value);

int
delete_SplayT(struct SplayTree_ST * SplayT,key_t key);

#endif















