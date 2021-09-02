
#include"../data_structure/Queue/queue.c"

struct Son_Node{
   char son_c;
   struct AC_am_Node * son_c_ptr;
   struct Son_Node * next;   
};

struct AC_am_Node{
   int count;
   int  key_index;
   char c;
   struct Son_Node * son_list;
   struct AC_am_Node * Fail;
};

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
numberOfKeys(struct AC_am_Node * Root,char * string);
