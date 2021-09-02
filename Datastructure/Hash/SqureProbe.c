
#include"./SqureProbe.h"

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
_insert(struct SP_Node ** new_addr,int new_size,struct SP_Node * spn){
   int index,i=0;
   struct SP_Node * spn_temp;

   index=Hash(spn->key,new_size);
   spn_temp=new_addr[index];
   while(spn_temp!=NULL){
      index=(index+2*++i-1)%new_size;
      spn_temp=new_addr[index];
   }
   new_addr[index]=spn;
}

static int
plusCap(struct SP_Table * SPT){
   struct SP_Node * spn,** new_addr;
   int index,new_size;

   new_size=next_prime(SPT->size *2 +1);
   if((new_addr=malloc(sizeof(struct SP_Node *)*new_size))==NULL)
       return -1;
   memset(new_addr,0,sizeof(struct SP_Node *) * new_size);
   for(index=0;index<SPT->size;index++){
       if(SPT->addr[index]==NULL)
           continue;
       if(SPT->addr[index]->flag==0){
           free(SPT->addr[index]);
           continue;
       }
       _insert(new_addr,new_size,SPT->addr[index]);
   }
   SPT->size=new_size;
   free(SPT->addr);
   SPT->addr=new_addr;

   return 0;
}

struct SP_Table *
init_SPTable(enum SP_Type lp_type){
   struct SP_Table * table;
   int size;
   
   size=next_prime(INIT_SIZE);
   if((table=malloc(sizeof(struct SP_Table)))==NULL)
       return NULL;
   if((table->addr=malloc(sizeof(struct SP_Node *)*size))==NULL)
       return NULL;
   table->size=size;
   table->cur_count=0;
   table->type=lp_type;
   memset(table->addr,0,sizeof(struct SP_Node * ) * size);

   return table;
}

int
SPT_insert(struct SP_Table * SPT,char * key,void * value){
   struct SP_Node * spn;
   int index,i=0;

   if(SPT->size<=SPT->cur_count * 2){
       if(plusCap(SPT)!=0)
           return -1;
   }

   index=Hash(key,SPT->size);
   spn=SPT->addr[index];
   while(spn!=NULL){
       if(spn->flag==0){
           strncpy(spn->key,key,KEY_LEN);
           spn->key[KEY_LEN-1]='\0';
           spn->value=value;
           spn->flag=1;
           SPT->cur_count++;
           return 0;
       }
       if(strcmp(key,spn->key)==0)
           return 1;
       index=(index+2*++i-1)%SPT->size;
       spn=SPT->addr[index];
   }
   if((spn=malloc(sizeof(struct SP_Node )))==NULL)
       return -1;
   strncpy(spn->key,key,KEY_LEN);
   spn->key[KEY_LEN-1]='\0';
   spn->value=value;
   spn->flag=1;
   SPT->addr[index]=spn;
   SPT->cur_count++;

   printf("index:%d\n",index);
   return 0;
}

int
SPT_delete(struct SP_Table * SPT,char * key){
   struct SP_Node * spn;
   int index,i=0;

   index=Hash(key,SPT->size);
   spn=SPT->addr[index];
   while(spn!=NULL){
       if(strcmp(key,SPT->addr[index]->key)==0)
           break;
       index=(index+2*++i-1)%SPT->size;
       spn=SPT->addr[index];
   }
   if(spn==NULL || spn->flag==0)
       return 1;
   spn->flag=0;
   SPT->cur_count--;  
   return 0;
}

int
SPT_get(struct SP_Table * SPT,char * key,void * rt_value){
   struct SP_Node * spn;
   int index,i=0;

   index=Hash(key,SPT->size);
   spn=SPT->addr[index];
   while(spn!=NULL){
       if(strcmp(key,SPT->addr[index]->key)==0)
           break;
       index=(index+2*++i-1)%SPT->size;
       spn=SPT->addr[index];
   }
   if(spn==NULL || spn->flag==0)
       return 1;
   switch(SPT->type){
       case CHAR:
           *(char *)rt_value=(char)spn->value;
           break;
       case INT:
           *(int *)rt_value=(int)spn->value;
           break;
       case LONG:
           *(long *)rt_value=(long)spn->value;
           break;
       case PTR:
           *(unsigned long *)rt_value=(unsigned long)spn->value;
           break;
       default:
           return -1;
   }

   return 0;
           
}

int
SPT_containKey(struct SP_Table * SPT,char * key){
   struct SP_Node * spn;
   int index,i=0;

   index=Hash(key,SPT->size);
   spn=SPT->addr[index];
   while(spn!=NULL){
       if(strcmp(key,SPT->addr[index]->key)==0)
           break;
       index=(index+2*++i-1)%SPT->size;
       spn=SPT->addr[index];
   }
   if(spn==NULL || spn->flag==0)
       return -1;
   return index;        
}













