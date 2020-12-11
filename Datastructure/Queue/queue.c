#include"./queue.h"
#include"../ds_tools.h"

int
create_queue(struct Queue * queue,enum _TYPE queue_type){
    queue->queue_type_len=queue_type?(queue_type==INT?sizeof(int):sizeof(long)):sizeof(char);
    if((queue->queue_p=malloc(QUEUE_INIT_SIZE*queue->queue_type_len))==NULL)
        return -1;
    queue->queue_type=queue_type;
    queue->queue_tail=queue->queue_head=0;
    queue->queue_size=QUEUE_INIT_SIZE;
    
    return 0;
}

int
queue_pop(struct Queue * queue,void * rt_value){
    if(queue->queue_head==queue->queue_tail)          //queue is empty!
        return -1;
    switch(queue->queue_type){
         case CHAR:         
             To_char(queue->queue_p+queue->queue_head*queue->queue_type_len,rt_value);
             break;
         case INT:
             To_int(queue->queue_p+queue->queue_head*queue->queue_type_len,rt_value);
             break;
         case LONG:
             To_long(queue->queue_p+queue->queue_head*queue->queue_type_len,rt_value);
             break;
         case PTR:
             To_ptr(queue->queue_p+queue->queue_head*queue->queue_type_len,rt_value);
             break;
         default:
             return -2;
    }
    return queue->queue_head=(queue->queue_head+1)%queue->queue_size;  
}

int
queue_push(struct Queue * queue,unsigned long value){
   if((queue->queue_head-1)%queue->queue_size==queue->queue_tail){
       int i;
       int * temp;
       if((temp=malloc(queue->queue_size*2*queue->queue_type_len))==NULL)
           return -1;
       i=queue->queue_size-queue->queue_head;
       memcpy(temp,queue->queue_p+queue->queue_head*queue->queue_type_len,i*queue->queue_type_len);
       if(queue->queue_tail<queue->queue_head){
           memcpy(temp+i,queue->queue_p,queue->queue_tail*queue->queue_type_len);  //because the tail hasn't data!
           i+=queue->queue_tail;     
       }
       queue->queue_head=0;
       queue->queue_tail=i;
       queue->queue_size*=2;
       free(queue->queue_p);
       queue->queue_p=temp;
   }
   switch(queue->queue_type){
         case CHAR:         
             To_char(&value,queue->queue_p+queue->queue_tail*queue->queue_type_len);
             break;
         case INT:
             To_int(&value,queue->queue_p+queue->queue_tail*queue->queue_type_len);
             break;
         case LONG:
             To_long(&value,queue->queue_p+queue->queue_tail*queue->queue_type_len);
             break;
         case PTR:
             To_ptr(&value,queue->queue_p+queue->queue_tail*queue->queue_type_len);
             break;
         default:
             return -2;
   }
   return queue->queue_tail=(queue->queue_tail+1)%queue->queue_size;

}

void
queue_release(struct Queue * queue){
    free(queue->queue_p);       
}






