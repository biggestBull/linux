#include"./Link.h"

static int
next_prime(int size){
   int i=0,sq_rt;
   sq_rt=floor(sqrt(size));

   while(i<=sq_rt){
       size++;
       sq_rt=floor(sqrt(size));
       for(i=2;i<=sq_rt;i++){
           if(size % i==0)
               break;
       }

   }
   printf("next prime:%d\n",size);
   return size;   
}

static int
Hash(char * key,int size){
   int sum=0;
  // if(key[KEY_LEN-1]!='\0')                //如果不为斜杠0，则必非常量
  //     key[KEY_LEN-1]='\0';
   while(*key!='\0'){
       sum=sum*32+*key++;     
   }
   return sum % size; 
}

static void
_insertToNew(struct HashByLink_Node ** new_addr,int new_size,struct HashByLink_Node* addr){
   struct HashByLink_Node * hln;
   int index;
   index=Hash(addr->key,new_size);
   hln=new_addr[index];
   
   addr->next=NULL;
   if(hln==NULL){
       new_addr[index]=addr;
   }else{
       while(hln->next!=NULL){
          hln=hln->next;
       }
       hln->next=addr;
   }
}

static int
HLT_plusCap(struct HashByLink_Table * HLT){
  int new_prime,i;
  struct HashByLink_Node ** new_addr,*temp,*temp_next;

  new_prime=next_prime(HLT->size*2+1);
  if((new_addr=malloc(new_prime*sizeof(struct HashByLink_Node *)))==NULL)
      return -1;
  memset(new_addr,0,new_prime*sizeof(struct HashByLink_Node *));
  for(i=0;i<HLT->size;i++){
      if((temp=HLT->addr[i])==NULL)
          continue;
      while(temp!=NULL){
         temp_next=temp->next;
         _insertToNew(new_addr,new_prime,temp); 
         temp=temp_next;    
      }
  }
  printf("oldsize:%d,newsize:%d\n",HLT->size,new_prime);
  HLT->size=new_prime;
  free(HLT->addr);
  HLT->addr=new_addr;
  return 0;
}

struct HashByLink_Table *
init_Hash_Table(enum Value_Type value_type){
   struct HashByLink_Table * HLT;
   
   if((HLT=malloc(sizeof(struct HashByLink_Table)))==NULL)
       return NULL;
   HLT->size=next_prime(INIT_SIZE);
   
   if((HLT->addr=malloc(HLT->size*sizeof(struct HashByLink_Node*)))==NULL){
	   free(HLT);
       return NULL;
	}
   HLT->value_type=value_type;
   HLT->cur_count=0;
   memset(HLT->addr,0,HLT->size*sizeof(struct HashByLink_Node *));

   return HLT;
}

int
HLT_insert(struct HashByLink_Table * HLT,char * key,void * value){
   struct HashByLink_Node * hln,*pre_hln;
   int index;

   if(HLT->size==HLT->cur_count)
       HLT_plusCap(HLT);

   index=Hash(key,HLT->size);
   hln=pre_hln=HLT->addr[index];
  
   while(hln!=NULL){
       if(strcmp(hln->key,key)==0)
           return 1;
       pre_hln=hln;
       hln=hln->next;
   }
   if((hln=malloc(sizeof(struct HashByLink_Node)))==NULL)
       return -1;
   strncpy(hln->key,key,KEY_LEN);
   hln->key[KEY_LEN-1]='\0';
   hln->value=value;
   hln->next=NULL;
   
   if(pre_hln==NULL){
      HLT->addr[index]=hln;     
   }else{
       pre_hln->next=hln;     
   }
   HLT->cur_count++;
   return 0;
}

int
HLT_delete(struct HashByLink_Table * HLT,char * key){
   struct HashByLink_Node * hln,*pre_hln;
   int index;

   index=Hash(key,HLT->size);
   pre_hln=hln=HLT->addr[index];
   while(hln!=NULL){
       if(strcmp(hln->key,key)==0)
           break;
       pre_hln=hln;
       hln=hln->next;
   }
   if(hln==NULL)
       return 1;

   
   if(pre_hln==NULL){
      HLT->addr[index]=hln->next;     
   }else{
       pre_hln->next=hln->next;     
   }
   free(hln);
   HLT->cur_count--;
   return 0;
           
}

int
HLT_get(struct HashByLink_Table * HLT,char * key,void * rt_value){
   struct HashByLink_Node * hln;
   int index;

   index=Hash(key,HLT->size);
   printf("index:%d\n",index);
   hln=HLT->addr[index];
   while(hln!=NULL){
       if(strcmp(hln->key,key)==0)
           break;
       hln=hln->next;
   }
   if(hln){
       switch(HLT->value_type){
           case CHAR:
               *(char *)rt_value=hln->value;
               break;
           case INT:
               *(int *)rt_value=hln->value;
               break;
           case LONG:
               *(long *)rt_value=hln->value;
               break;
           case PTR:
               *(unsigned long *)rt_value=hln->value;
               break;
           default:
               return -1;
               
       }
       return 0;
   }
   return 1;


}

int
HLT_containKey(struct HashByLink_Table * HLT,char * key){
   struct HashByLink_Node * hln;
   int index;

   index=Hash(key,HLT->size);
   printf("index:%d\n",index);
   hln=HLT->addr[index];
   printf("%ld\n",hln);
   while(hln!=NULL){
       if(strcmp(hln->key,key)==0)
           break;
       hln=hln->next;
   }
   return hln==NULL?-1:index;
}






















