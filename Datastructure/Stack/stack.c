#include"./stack.h"
#include"../ds_tools.h"

#define STACK_INIT_SIZE 100

int
create_stack(struct Stack * stack,enum _TYPE stack_type){
   stack->stack_type_len=stack_type?(stack_type==INT?sizeof(int):sizeof(long)):sizeof(char);
   if((stack->stack_p=malloc(STACK_INIT_SIZE*stack->stack_type_len))==NULL)
       return -1;
   stack->stack_size=STACK_INIT_SIZE;
   stack->stack_type=stack_type;
   stack->stack_top=-1;
   return 0;            //万万没想到啊，之前居然漏写了返回值了，而且竟然不报错，似乎gcc默认返回一个垃圾值，凶险。
}

int
stack_pop(struct Stack * stack,void * rt_value){
   if(stack->stack_top<0)
       return -1;            //stack is empty!
  
   switch(stack->stack_type){
       case CHAR:         
         To_char(stack->stack_top*stack->stack_type_len+stack->stack_p,rt_value);
         break;
       case INT:
         To_int(stack->stack_top*stack->stack_type_len+stack->stack_p,rt_value);
         break;
       case LONG:
         To_long(stack->stack_top*stack->stack_type_len+stack->stack_p,rt_value);
         break;
       case PTR:
         To_ptr(stack->stack_top*stack->stack_type_len+stack->stack_p,rt_value);
         break;
       default:
         return -2;

   }
   return stack->stack_top--;
   
}

int
stack_push(struct Stack * stack,unsigned long value){
   if(stack->stack_top>=stack->stack_size-1){
       int * temp;
       if((temp=malloc(stack->stack_size*2*stack->stack_type_len))==NULL)
           return -1;
       memcpy(temp,stack->stack_p,stack->stack_size*stack->stack_type_len);
       free(stack->stack_p);
       stack->stack_p=temp;
       stack->stack_size=stack->stack_size*2;
   }
   ++(stack->stack_top);
   switch(stack->stack_type){
       case CHAR:         
         To_char(&value,stack->stack_p+stack->stack_top*stack->stack_type_len);
         break;
       case INT:
         To_int(&value,stack->stack_p+stack->stack_top*stack->stack_type_len);
         break;
       case LONG:
         To_long(&value,stack->stack_p+stack->stack_top*stack->stack_type_len);
         break;
       case PTR:
         To_ptr(&value,stack->stack_p+stack->stack_top*stack->stack_type_len);
         break;
       default:
         return -2;

   }
   return stack->stack_top;
}









