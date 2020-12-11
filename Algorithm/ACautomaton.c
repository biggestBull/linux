#include"./Algorithm.h"

static int 
_insert_key(struct AC_am_Node * Root,char * key){
    struct Son_Node * temp_son;
    struct AC_am_Node * temp_acam=Root;
    int index=0;

    while(key[index]!='\0'){
         temp_son=temp_acam->son_list;
         while(temp_son!=NULL){
            if(temp_son->son_c==key[index]){
               temp_acam=temp_son->son_c_ptr;
               if(key[index+1]=='\0'){
                  temp_acam->count=0;
                  temp_acam->key=key;      
               }else{
                  temp_acam->key=NULL;       
               }
               index++;
               break;        
            }
            temp_son=temp_son->next;
         }
         if(temp_son!=NULL)
             continue;
         if((temp_son=malloc(sizeof(struct Son_Node)))==NULL)
             return -1;
        
         if((Root=malloc(sizeof(struct AC_am_Node)))==NULL)
             return -1;

         temp_son->next=temp_acam->son_list;
         temp_acam->son_list=temp_son;
         temp_son->son_c=key[index];
         temp_son->son_c_ptr=Root;
         if(key[index+1]=='\0'){
            Root->count=0;
            Root->key=key;      
         }else{
            Root->count=-1;
            Root->key=NULL;       
         }
         Root->c=key[index];
         Root->son_list=NULL;
         temp_acam=Root;
         index++;
    }
    return 0;
}

static void
_delete_ACAM(struct AC_am_Node * Root,int flag){
    struct AC_am_Node * temp_acam;
    struct Son_Node * temp_son;
    struct Queue queue;

    if(create_queue(&queue,PTR)!=-1){
        queue_push(&queue,Root);
        while(queue_pop(&queue,&temp_acam)>=0){
           temp_acam->count=temp_acam->count<0?-1:0;
           temp_son=temp_acam->son_list;
           while(temp_son!=NULL){
               struct Son_Node * pre_son;
               queue_push(&queue,temp_son->son_c_ptr);
               pre_son=temp_son;
               temp_son=temp_son->next;
               if(flag)
                   free(pre_son);
           }
           if(flag)
              free(temp_acam);
        }
    }
    queue_release(&queue);
}

static int
_construct_Fail(struct AC_am_Node * Root){
    struct AC_am_Node * temp_acam=Root;
    struct Son_Node * temp_son1,*temp_son2;
    struct Queue queue;

    if(create_queue(&queue,PTR)==-1)
        return -1;
    temp_acam->Fail=NULL;
    temp_son1=temp_acam->son_list;
    while(temp_son1!=NULL){
        temp_acam=temp_son1->son_c_ptr;
        temp_acam->Fail=Root;
        temp_son2=temp_acam->son_list;
        while(temp_son2!=NULL){
           temp_son2->son_c_ptr->Fail=temp_acam;
           queue_push(&queue,temp_son2->son_c_ptr);
           temp_son2=temp_son2->next;
        }
        temp_son1=temp_son1->next;
    }
    while(queue_pop(&queue,&temp_acam)>=0){
        Root=temp_acam->Fail->Fail;
        while(Root!=NULL){
            temp_son1=Root->son_list;
            temp_acam->Fail=Root;
            while(temp_son1!=NULL){
                if(temp_son1->son_c==temp_acam->c){
                   temp_acam->Fail=temp_son1->son_c_ptr;
                   break;
                }
                temp_son1=temp_son1->next;
            }
            if(temp_son1!=NULL)
                break;
            Root=Root->Fail;
        }
        temp_son2=temp_acam->son_list;
        while(temp_son2!=NULL){
           temp_son2->son_c_ptr->Fail=temp_acam;
           queue_push(&queue,temp_son2->son_c_ptr);
           temp_son2=temp_son2->next;
        }
    }
    queue_release(&queue);
    return 0;
}

int
delete_key(struct AC_am_Node * Root,char * key){
    struct AC_am_Node * temp_acam=Root;
    struct Son_Node * temp_son;

    while(*key!='\0'){
        temp_son=temp_acam->son_list;
        while(temp_son!=NULL){
           if(temp_son->son_c==*key){
               temp_acam=temp_son->son_c_ptr;
               key++;
               break;     
           }
           temp_son=temp_son->next;
        }
        if(temp_son!=NULL)
            continue;
        return -1;
    }
    temp_acam->count=-1;
   // temp_acam->key=NULL;
    return 0;
}

int
insert_key(struct AC_am_Node * Root,char * key){
   int rt_value;
  
   rt_value=_insert_key(Root,key);
   _construct_Fail(Root);
   return rt_value;
}

void
delete_ACAM(struct AC_am_Node * Root){
   _delete_ACAM(Root,1);        
}

struct AC_am_Node *
create_ACAM(char ** keyset){
   char * key;
   struct AC_am_Node * Root;
   int rt_value;

   if((Root=malloc(sizeof(struct AC_am_Node )))==NULL)
       return NULL;
   Root->Fail=NULL;
   Root->count=-1;
   Root->c='\0';
   Root->son_list=NULL;
   
   key=*keyset;
   while(key!=NULL){
      rt_value=_insert_key(Root,key);      
      if(rt_value==-1){
          printf("1.1\n");
          delete_ACAM(Root);   
          return NULL;
      }
      key=*(++keyset);
   }
   _construct_Fail(Root);
   return Root;
}

int
isKeyExist(struct AC_am_Node * Root,char * string){
   struct AC_am_Node * temp_acam=Root;
   struct Son_Node * temp_son;
   int index=0;

   while(string[index]!='\0'){
       temp_son=temp_acam->son_list;
       if(temp_acam->count!=-1)
           return index;
       while(temp_son!=NULL){
           if(temp_son->son_c==string[index]){
               index++;
               temp_acam=temp_son->son_c_ptr;
               break;
           }
           temp_son=temp_son->next;
       }
       if(temp_son!=NULL)
           continue;
       if(temp_acam->Fail==NULL){
           index++;     
       }else{
           temp_acam=temp_acam->Fail;     
       }

   }
   return -1;
}

struct Queue *
numberOfKeys(struct AC_am_Node * Root,char * string,struct Queue *queue){
   struct AC_am_Node * temp_acam=Root,*temp_end=NULL;
   struct Son_Node * temp_son;
   int index=0;

   _delete_ACAM(Root,0);
   while(string[index]!='\0'){
       temp_son=temp_acam->son_list;
       while(temp_son!=NULL){
           if(temp_son->son_c==string[index]){
               index++;
               temp_acam=temp_son->son_c_ptr;
               if(temp_acam->count>=0){
                   if(temp_acam->count==0)
                       queue_push(queue,temp_acam);
                   temp_end=temp_acam;
                   temp_end->key_index=index;
               }
               break;
           }
           temp_son=temp_son->next;
       }
       if(temp_son!=NULL)
           continue;
       if(temp_end!=NULL){
           temp_end->count++;
           index=temp_end->key_index;
           temp_acam=Root;
           temp_end=NULL;
           continue;
       }
       if(temp_acam->Fail==NULL){
           index++;     
       }else{
           temp_acam=temp_acam->Fail;     
       }

   }
 
   return queue;
}
