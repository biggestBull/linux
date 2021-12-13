
#include"./DoubleHash.h"

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

static int
Hash2(int hash1,int hash2prime){                    //我也不知道hash2的素数是不是固定的，暂时当作是好了
   return hash2prime-(hash1 % hash2prime);        
}

static void
_insert(struct DH_Node ** new_addr,int new_size,int hash2,struct DH_Node * dhn){
   int index;
   struct DH_Node * dhn_temp;

   index=Hash(dhn->key,new_size);

   dhn_temp=new_addr[index];
   while(dhn_temp!=NULL){
      index=(index+hash2)%new_size;
      dhn_temp=new_addr[index];
   }
   new_addr[index]=dhn;
}

static int
plusCap(struct DH_Table * DHT){
   struct DH_Node * dhn,** new_addr;
   int index,new_size;

   new_size=next_prime(DHT->size *2 +1);
   if((new_addr=malloc(sizeof(struct DH_Node *)*new_size))==NULL)
       return -1;
   memset(new_addr,0,sizeof(struct DH_Node *) * new_size);
   
   DHT->hash2prime=next_prime(new_size/2);
   for(index=0;index<DHT->size;index++){
       if(DHT->addr[index]==NULL)
           continue;
       if(DHT->addr[index]->flag==0){
           free(DHT->addr[index]);
           continue;
       }
       _insert(new_addr,new_size,Hash2(index,DHT->hash2prime),DHT->addr[index]);
   }
   DHT->size=new_size;
   free(DHT->addr);
   DHT->addr=new_addr;

   return 0;
}

struct DH_Table *
init_DHTable(enum DH_Type lp_type){
   struct DH_Table * table;
   int size;
   
   size=next_prime(INIT_SIZE);
   if((table=malloc(sizeof(struct DH_Table)))==NULL)
       return NULL;
   if((table->addr=malloc(sizeof(struct DH_Node *)*size))==NULL){
	   free(table);
       return NULL;
	}
   table->hash2prime=next_prime(INIT_SIZE/2);
   table->size=size;
   table->cur_count=0;
   table->type=lp_type;
   memset(table->addr,0,sizeof(struct DH_Node * ) * size);

   return table;
}

int
DHT_insert(struct DH_Table * DHT,char * key,void * value){
   struct DH_Node * dhn;
   int index,hash2;

   if(DHT->size<=DHT->cur_count * 2){
       if(plusCap(DHT)!=0)
           return -1;
   }

   index=Hash(key,DHT->size);
   hash2=Hash2(index,DHT->hash2prime);
   dhn=DHT->addr[index];
   printf("index:%d,hash2:%d\n",index,hash2);
   while(dhn!=NULL){
       if(dhn->flag==0){
           strncpy(dhn->key,key,KEY_LEN);
           dhn->key[KEY_LEN-1]='\0';
           dhn->value=value;
           dhn->flag=1;
           DHT->cur_count++;
           return 0;
       }
       if(strcmp(key,dhn->key)==0)
           return 1;
       index=(index+hash2)%DHT->size;
       dhn=DHT->addr[index];
   }
   if((dhn=malloc(sizeof(struct DH_Node )))==NULL)
       return -1;
   strncpy(dhn->key,key,KEY_LEN);
   dhn->key[KEY_LEN-1]='\0';
   dhn->value=value;
   dhn->flag=1;
   DHT->addr[index]=dhn;
   DHT->cur_count++;

   return 0;
}

int
DHT_delete(struct DH_Table * DHT,char * key){
   struct DH_Node * dhn;
   int index,hash2;

   index=Hash(key,DHT->size);
   hash2=Hash2(index,DHT->hash2prime);
   dhn=DHT->addr[index];
   while(dhn!=NULL){
       if(strcmp(key,DHT->addr[index]->key)==0)
           break;
       index=(index+hash2)%DHT->size;
       dhn=DHT->addr[index];
   }
   if(dhn==NULL || dhn->flag==0)
       return 1;
   dhn->flag=0;
   DHT->cur_count--;  
   return 0;
}

int
DHT_get(struct DH_Table * DHT,char * key,void * rt_value){
   struct DH_Node * dhn;
   int index,hash2;

   index=Hash(key,DHT->size);
   hash2=Hash2(index,DHT->hash2prime);
   dhn=DHT->addr[index];
   while(dhn!=NULL){
       if(strcmp(key,DHT->addr[index]->key)==0)
           break;
       index=(index+hash2)%DHT->size;
       dhn=DHT->addr[index];
   }
   if(dhn==NULL || dhn->flag==0)
       return 1;
   switch(DHT->type){
       case CHAR:
           *(char *)rt_value=(char)dhn->value;
           break;
       case INT:
           *(int *)rt_value=(int)dhn->value;
           break;
       case LONG:
           *(long *)rt_value=(long)dhn->value;
           break;
       case PTR:
           *(unsigned long *)rt_value=(unsigned long)dhn->value;
           break;
       default:
           return -1;
   }

   return 0;
           
}

int
DHT_containKey(struct DH_Table * DHT,char * key){
   struct DH_Node * dhn;
   int index,hash2;

   index=Hash(key,DHT->size);
   hash2=Hash2(index,DHT->hash2prime);
   dhn=DHT->addr[index];
   while(dhn!=NULL){
       if(strcmp(key,DHT->addr[index]->key)==0)
           break;
       index=(index+hash2)%DHT->size;
       dhn=DHT->addr[index];
   }
   if(dhn==NULL || dhn->flag==0)
       return -1;
   return index;        
}













