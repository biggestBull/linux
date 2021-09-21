#ifndef LEFTHEAP_HEAD
#define LEFTHEAP_HEAD

#include<stdlib.h>
#include<string.h>

enum LH_Type{
   CHAR,
   INT,
   LONG
};

struct LH_Node{
   enum LH_Type type;
   int npl;
   int flag;
   void * value;
   void * (*func)(void *);
   struct LH_Node * left;
   struct LH_Node * right;
};

struct LH_Node *
create_LH(enum LH_Type type,int flag,void * value,void * (*func)(void *));
struct LH_Node *
merge(struct LH_Node * H1,struct LH_Node * H2);
int 
insert(struct LH_Node ** LHN,void * value);
int
pop(struct LH_Node ** LHN,void * rt_value,void * last_lhn);

#endif
