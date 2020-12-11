#ifndef STACK_HEADER

#define STACK_HEADER
#include"../type.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Stack{
   int stack_size;
   int stack_top;
   enum _TYPE stack_type;
   int stack_type_len;
   void * stack_p;
};


int create_stack(struct Stack *,enum _TYPE);
int stack_pop(struct Stack *,void *);
int stack_push(struct Stack * ,unsigned long);
#endif







