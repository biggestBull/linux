

#ifndef DOUBLEHASH_HEAD
#define DOUBLEHASH_HEAD

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define KEY_LEN 8
#define INIT_SIZE 1000

enum DH_Type{
   CHAR,
   INT,
   LONG,
   PTR
};

struct DH_Node{
   char key[KEY_LEN];
   void * value;
   int flag;
};

struct DH_Table{
   int size;
   int hash2prime;
   enum DH_Type type;
   int cur_count;
   struct DH_Node ** addr;
};

struct DH_Table *
init_DHTable(enum DH_Type lp_type);
int
DHT_insert(struct DH_Table * DHT,char * key,void * value);
int
DHT_delete(struct DH_Table * DHT,char * key);
int
DHT_get(struct DH_Table * DHT,char * key,void * rt_value);
int
DHT_containKey(struct DH_Table *,char *);
#endif

