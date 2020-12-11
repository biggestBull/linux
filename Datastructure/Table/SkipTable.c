#include"./SkipTable.h"

static int
_findPosition(struct _ST * st,int key){
   struct ST_Node * temp_d=st->head,* temp_n,*temp_p;

   clear_stack(&st->stack);
   while(temp_d){
      temp_p=temp_d;
      temp_n=temp_d->next;
      if(temp_n){
         while(temp_n){
            if(key<=temp_n->key){
               if(stack_push(&st->stack,temp_p)==-1)
                   return -1;
               break;
            }
            temp_p=temp_n;
            temp_n=temp_n->next;
         }
      }
      if(!temp_n){
         if(stack_push(&st->stack,temp_p)==-1)
            return -1;
      }
      temp_d=temp_d->down;
   }
   return 0;
}


struct _ST *
createST(int layer_num){
  struct _ST * st;
  struct ST_Node * temp;

  srand(time(NULL));
  if(layer_num<2)
      return NULL;

  if((st=malloc(sizeof(struct _ST)))==NULL)
     return NULL;
  if(create_stack(&st->stack,PTR)==-1)
      return NULL;
  st->layer_range=1<<layer_num-1;
  st->head=NULL;
  while(layer_num--){
     if((temp=malloc(sizeof(struct ST_Node)))==NULL){
        free(st);
        return NULL;
     }
     temp->next=NULL;
     temp->down=st->head;
     st->head=temp;
  }
  return st;
}

int
insertST(struct _ST * st,int key,void * value){
  struct ST_Node * temp1,*temp,*temp_p,*temp_n;
  int r,i;
  
  if(_findPosition(st,key)==-1)
      return -1;
  if(stack_pop(&st->stack,&temp_p)==-1){
      return -1;   
  }
  temp_n=temp_p->next;
  if(temp_n && temp_n->key==key){
      temp_n->value=value;
      return 1;
  }
  if((temp=malloc(sizeof(struct ST_Node)))==NULL)
      return -1;
  temp->down=NULL;
  temp->next=temp_n;
  temp_p->next=temp;
  temp->key=key;
  temp->value=value;
  

  i=st->layer_range;
  r=rand()%i;
  
  for(i/=2;i>r;i/=2){
     if(stack_pop(&st->stack,&temp_p)==-1)
         return -1;
     if((temp1=malloc(sizeof(struct ST_Node)))==NULL)
         return -1;
     temp1->down=temp;
     temp1->next=temp_p->next;
     temp_p->next=temp1;
     temp1->key=key;

     temp=temp1;
  }
  return 0;
}

int
deleteST(struct _ST * st,int key){
  struct ST_Node * temp,*temp_p,*temp_n;

  if(_findPosition(st,key)==-1)
      return -1;
  if(stack_pop(&st->stack,&temp_p)==-1)
      return -1;
  temp_n=temp_p->next;
  if(temp_n==NULL || temp_n->key!=key)
      return 1;
  temp_p->next=temp_n->next;
  free(temp_n);
  while(stack_pop(&st->stack,&temp_p)!=-1){
       temp_n=temp_p->next;
       if(temp_n==NULL || temp_n->key!=key)
           continue;
       temp_p->next=temp_n->next;
       free(temp_n);  
  }
  return 0;
}

int  
searchST(struct _ST * st,int key,void * rt_value){
  struct ST_Node * temp,*temp_p,*temp_n;

  if(_findPosition(st,key)==-1)
      return -1;
  if(stack_pop(&st->stack,&temp_p)==-1)
      return -1;
  temp_n=temp_p->next;
  if(temp_n==NULL || temp_n->key!=key)
      return 1;
  *((unsigned long *)(rt_value))=temp_n->value;
  return 0;         
}











