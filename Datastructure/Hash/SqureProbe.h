

#ifndef SQURE_HEAD
#define SQURE_HEAD

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define KEY_LEN 8
#define INIT_SIZE 1000

enum SP_Type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct SP_Node{
   char key[KEY_LEN];
   void * value;
   int flag;
};

struct SP_Table{
   int size;
   enum SP_Type type;
   int cur_count;
   struct SP_Node ** addr;
};

struct SP_Table *
init_SPTable(enum SP_Type lp_type);
int
SPT_insert(struct SP_Table * SPT,char * key,void * value);
int
SPT_delete(struct SP_Table * SPT,char * key);
int
SPT_get(struct SP_Table * SPT,char * key,void * rt_value);
int
SPT_containKey(struct SP_Table *,char *);
#endif

