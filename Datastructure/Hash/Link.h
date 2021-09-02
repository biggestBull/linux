#ifndef LINK_HEAD
#define Link_HEAD

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<string.h>

#define INIT_SIZE 1000
#define KEY_LEN 8

enum Value_Type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct HashByLink_Node{
   char key[KEY_LEN];
   void * value;
   struct HashByLink_Node * next;
};

struct HashByLink_Table{
   int size;
   int cur_count;
   enum Value_Type value_type;
   struct HashByLink_Node ** addr;
};

struct HashByLink_Table *
init_Hash_Table(enum Value_Type value_type);

int
HLT_insert(struct HashByLink_Table * HLT,char * key,void * value);
int
HLT_delete(struct HashByLink_Table * HLT,char * key);
int
HLT_get(struct HashByLink_Table * HLT,char * key,void * rt_value);
int
HLT_containKey(struct HashByLink_Table * HLT,char * key);

#endif
