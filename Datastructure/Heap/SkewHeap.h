#ifndef SkewHEAP_HEAD
#define SkewHEAP_HEAD

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum SH_Type{
   CHAR,
   INT,
   LONG
};

struct SH_Node{
   enum SH_Type type;
   int flag;
   void * value;
   void * (*func)(void *);
   struct SH_Node * left;
   struct SH_Node * right;
};

struct SH_Node *
create_SH(enum SH_Type type,int flag,void * value,void * (*func)(void *));
struct SH_Node *
merge(struct SH_Node * H1,struct SH_Node * H2);
int 
insert(struct SH_Node ** SHN,void * value);
int
pop(struct SH_Node ** SHN,void * rt_value,void * last_shn);

#endif
