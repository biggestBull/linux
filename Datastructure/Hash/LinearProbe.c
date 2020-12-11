#include"./LinearProbe.h"

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
       sum=sum*32+(unsigned char)*key++;     
   }
   return sum % size; 
}

static void
_insert(struct LP_Node ** new_addr,int new_size,struct LP_Node * lpn){
   int index;
   struct LP_Node * lpn_temp;

   index=Hash(lpn->key,new_size);
   lpn_temp=new_addr[index];
   while(lpn_temp!=NULL){
      index=(index+1)%new_size;
      lpn_temp=new_addr[index];
   }
   new_addr[index]=lpn;
}

static int
plusCap(struct LP_Table * LPT){
   struct LP_Node * lpn,** new_addr;
   int index,new_size;

   new_size=next_prime(LPT->size *2 +1);
   if((new_addr=malloc(sizeof(struct LP_Node *)*new_size))==NULL)
       return -1;
   memset(new_addr,0,sizeof(struct LP_Node *) * new_size);
   for(index=0;index<LPT->size;index++){
       if(LPT->addr[index]==NULL)
           continue;
       if(LPT->addr[index]->flag==0){
           free(LPT->addr[index]);
           continue;
       }
       _insert(new_addr,new_size,LPT->addr[index]);
   }
   LPT->size=new_size;
   free(LPT->addr);
   LPT->addr=new_addr;

   return 0;
}

struct LP_Table *
init_LPTable(enum LP_Type lp_type){
   struct LP_Table * table;
   int size;
   
   size=next_prime(INIT_SIZE);
   if((table=malloc(sizeof(struct LP_Table)))==NULL)
       return NULL;
   if((table->addr=malloc(sizeof(struct LP_Node *)*size))==NULL)
       return NULL;
   table->size=size;
   table->cur_count=0;
   table->type=lp_type;
   memset(table->addr,0,sizeof(struct LP_Node * ) * size);

   return table;
}

int
LPT_insert(struct LP_Table * LPT,char * key,void * value){
   struct LP_Node * lpn;
   int index;

   if(LPT->size<=LPT->cur_count * 2){
       if(plusCap(LPT)!=0)
           return -1;
   }

   index=Hash(key,LPT->size);
   lpn=LPT->addr[index];
   while(lpn!=NULL){
       if(lpn->flag==0){
           strncpy(lpn->key,key,KEY_LEN);
           lpn->key[KEY_LEN-1]='\0';
           lpn->value=value;
           lpn->flag=1;
           LPT->cur_count++;
           return 0;
       }
       if(strcmp(key,lpn->key)==0)
           return 1;
       index=(index+1)%LPT->size;
       lpn=LPT->addr[index];
   }
   if((lpn=malloc(sizeof(struct LP_Node )))==NULL)
       return -1;
   strncpy(lpn->key,key,KEY_LEN);
   lpn->key[KEY_LEN-1]='\0';
   lpn->value=value;
   lpn->flag=1;
   LPT->addr[index]=lpn;
   LPT->cur_count++;

   printf("index:%d\n",index);
   return 0;
}

int
LPT_delete(struct LP_Table * LPT,char * key){
   struct LP_Node * lpn;
   int index;

   index=Hash(key,LPT->size);
   lpn=LPT->addr[index];
   while(lpn!=NULL){
       if(strcmp(key,LPT->addr[index]->key)==0)
           break;
       index=(index+1)%LPT->size;
       lpn=LPT->addr[index];
   }
   if(lpn==NULL || lpn->flag==0)
       return 1;
   lpn->flag=0;
   LPT->cur_count--;  
   return 0;
}

int
LPT_get(struct LP_Table * LPT,char * key,void * rt_value){
   struct LP_Node * lpn;
   int index;

   index=Hash(key,LPT->size);
   lpn=LPT->addr[index];
   while(lpn!=NULL){
       if(strcmp(key,LPT->addr[index]->key)==0)
           break;
       index=(index+1)%LPT->size;
       lpn=LPT->addr[index];
   }
   if(lpn==NULL || lpn->flag==0)
       return 1;
   switch(LPT->type){
       case CHAR:
           *(char *)rt_value=(char)lpn->value;
           break;
       case INT:
           *(int *)rt_value=(int)lpn->value;
           break;
       case LONG:
           *(long *)rt_value=(long)lpn->value;
           break;
       case PTR:
           *(unsigned long *)rt_value=(unsigned long)lpn->value;
           break;
       default:
           return -1;
   }

   return 0;
           
}

int
LPT_containKey(struct LP_Table * LPT,char * key){
   struct LP_Node * lpn;
   int index;

   index=Hash(key,LPT->size);
   lpn=LPT->addr[index];
   while(lpn!=NULL){
       if(strcmp(key,LPT->addr[index]->key)==0)
           break;
       index=(index+1)%LPT->size;
       lpn=LPT->addr[index];
   }
   if(lpn==NULL || lpn->flag==0)
       return -1;
   return index;        
}













