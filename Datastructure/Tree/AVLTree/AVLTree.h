#ifndef AVL_HEAD
#define AVL_HEAD

#include<stdlib.h>

typedef unsigned long ul_ptr_t;
typedef int key_t;

enum Value_Type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct AVLTree_Node{
  key_t key;
  void * value;
  int height;
  struct AVLTree_Node * left;
  struct AVLTree_Node * right;
};

struct AVLTree_ST{
  enum Value_Type value_type;
  struct AVLTree_Node * Root;	
};

struct AVLTree_ST *
create_AVLT(enum Value_Type value_type);

void
free_AVLT(struct AVLTree_ST * AVLT);

int
search_AVLT(struct AVLTree_ST * AVLT,key_t key,void * rt_value);

struct AVLTree_Node *
insert_AVLT(struct AVLTree_ST * AVLT,key_t key,void * value);

struct AVLTree_Node *
delete_AVLT(struct AVLTree_ST * AVLT,key_t key);

#endif
