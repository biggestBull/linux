#ifndef ALG_STRING_HEAD
#define ALG_STRING_HEAD

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../../Datastructure/Queue/queue.h"


struct Son_Node{
   char son_c;
   struct AC_am_Node * son_c_ptr;
   struct Son_Node * next;   
};

struct AC_am_Node{
   int count;
   int  key_index;
   char c;
   char * key;
   struct Son_Node * son_list;
   struct AC_am_Node * Fail;
};

int                                 //   if true,return index of the string
_KMP(const char * string,const char * key_string,int * fail_arr);


int
delete_key(struct AC_am_Node * Root,char * key);        
int
insert_key(struct AC_am_Node * Root,char * key);
void
delete_ACAM(struct AC_am_Node * Root);
struct AC_am_Node *
create_ACAM(char ** keyset);
int
isKeyExist(struct AC_am_Node * Root,char * string);
struct Queue *

numberOfKeys(struct AC_am_Node * Root,char * string,struct Queue *queue);

#endif
