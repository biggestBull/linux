
#ifndef LINEAR_HEAD
#define LINEAR_HEAD

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define KEY_LEN 8
#define INIT_SIZE 1000

enum LP_Type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct LP_Node{
   char key[KEY_LEN];
   void * value;
   int flag;
};

struct LP_Table{
   int size;
   enum LP_Type type;
   int cur_count;
   struct LP_Node ** addr;
};

struct LP_Table *
init_LPTable(enum LP_Type lp_type);
int
LPT_insert(struct LP_Table * LPT,char * key,void * value);
int
LPT_delete(struct LP_Table * LPT,char * key);
int
LPT_get(struct LP_Table * LPT,char * key,void * rt_value);
int
LPT_containKey(struct LP_Table *,char *);
#endif

