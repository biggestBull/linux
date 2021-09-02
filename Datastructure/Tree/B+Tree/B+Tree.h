#ifndef B_43_TREE_H
#define B_43_TREE_H

#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

#define B43_M 32 

struct Key_Ptr{
   int key;
   off_t ptr;
};

struct B43Tree_Node{
   off_t parent;
   int cur_key_num;
   int isleave;
   struct Key_Ptr sons[B43_M+1];
   off_t other_son;
};

struct B43Tree{
   int fd;
   int data_size;
   off_t offset;
   struct B43Tree_Node Root;
};

struct B43Tree *
createB43Tree(char * filename,int data_size);
struct B43Tree *
openB43Tree(char * filename);
int
B43Tinsert(struct B43Tree * BT,int key,void * data);
int
B43Tsearch(struct B43Tree * BT,int key,void * data_p);
int
B43Tdelete(struct B43Tree * BT,int key);
#endif
