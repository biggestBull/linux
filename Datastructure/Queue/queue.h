#ifndef QUEUE_HEAD
#define QUEUE_HEAD

#include"../type.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define QUEUE_INIT_SIZE 100

struct Queue{
   int queue_size;
   int queue_head;
   int queue_tail;
   enum _TYPE queue_type;
   int queue_type_len;
   void * queue_p;
};

int create_queue(struct Queue * queue,enum _TYPE queue_type);
int queue_pop(struct Queue * queue,void * rt_value);
int queue_push(struct Queue * queue,unsigned long value);
void queue_release(struct Queue *);

#endif 
