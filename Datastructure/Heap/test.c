#include<errno.h>
#include"./SkewHeap.h"

struct SH_Node *
create_SH(enum SH_Type type,int flag,void * value,void * (*func)(void *));
struct SH_Node *
merge(struct SH_Node * H1,struct SH_Node * H2);
int 
insert(struct SH_Node ** SHN,void * value);
int
pop(struct SH_Node ** SHN,void * rt_value,void * last_lhn);

struct test{
   char value_char;
   int value_int;  
   long value_long;
};

void *
some_func_char(void * addr){
   return ((struct test *)(addr))->value_char;        
}
void *
some_func_int(void * addr){
   return ((struct test *)(addr))->value_int;        
}
void *
some_func_long(void * addr){
   return ((struct test *)(addr))->value_long;        
}



int
main(int argc,char * argv[]){
    struct LeftHeap * heap_char,*heap_int,*heap_long,*temp=NULL;
    struct LeftHeap * heap_char_ptr,*heap_int_ptr,*heap_long_ptr;
    struct test * value;

    char arr_char[10]={
       1,2,3,4,5,6,7,8,9,10        
    };
    int arr_int[10]={
       1,2,3,4,5,6,7,8,9,10        
    };
    long arr_long[10]={
       1,2,3,4,5,6,7,8,9,10        
    };

    int i;
    struct test test[10],*test_ptr[10];
    for(i=0;i<10;i++){
       test[i].value_char=test[i].value_int=test[i].value_long=i;        
       test_ptr[i]=test+i;
    }
               

heap_char=create_SH(CHAR,1,test,some_func_char);
heap_int=create_SH(INT,1,test,some_func_int);
heap_long=create_SH(LONG,1,test,some_func_long);
 
printf("insert:%d,%d\n",insert(&heap_char,test+1),test[1].value_char);
printf("insert:%d,%d\n",insert(&heap_char,test+5),test[5].value_char);
printf("insert:%d,%d\n",insert(&heap_char,test+2),test[2].value_char);
printf("insert:%d,%d\n",insert(&heap_char,test),test[0].value_char);
printf("insert:%d,%d\n",insert(&heap_char,test+4),test[4].value_char);
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));
printf("value:%d,last:%p,pop:%d\n",value->value_char,heap_char,pop(&heap_char,&value,&temp));

printf("------------------------------------------------\n");
printf("insert:%d,%d\n",insert(&heap_int,test+1),test+1);
printf("insert:%d,%d\n",insert(&heap_int,test+5),test+5);
printf("insert:%d,%d\n",insert(&heap_int,test+2),test+2);
printf("insert:%d,%d\n",insert(&heap_int,test),test);
printf("insert:%d,%d\n",insert(&heap_int,test+4),test+4);
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_int,heap_int,pop(&heap_int,&value,&temp));
printf("------------------------------------------------\n");
printf("insert:%d,%d\n",insert(&heap_long,test+1),test+1);
printf("insert:%d,%d\n",insert(&heap_long,test+5),test+5);
printf("insert:%d,%d\n",insert(&heap_long,test+2),test+2);
printf("insert:%d,%d\n",insert(&heap_long,test),test);
printf("insert:%d,%d\n",insert(&heap_long,test+4),test+4);
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
printf("value:%p,last:%p,pop:%d\n",value->value_long,heap_long,pop(&heap_long,&value,&temp));
}
