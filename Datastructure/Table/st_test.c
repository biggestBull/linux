#include<stdio.h>
#include"./SkipTable.h"

int
main(){
   unsigned long test=0;
   struct _ST *st;
   int i,j;
   struct ST_Node * temp_d,* temp_n;

   printf("create:%p\n",st=createST(4));
   for(i=0;i<20;i++){
        test=i;
        printf("insert:%d,key:%d,value:%ld\n",insertST(st,j,test),j=rand()%10,test);
   }
   temp_d=st->head;
   while(temp_d){
       temp_n=temp_d->next;
       while(temp_n){
           printf("[%d %ld] ",temp_n->key,(unsigned long)(temp_n->value));    
           temp_n=temp_n->next;
       }
       printf("\n");
       temp_d=temp_d->down;
   }
   for(i=0;i<20;i++){
        printf("key:%d,seach:%ld,rt:%d\n",i,test,searchST(st,i,&test));
   }
   for(i=1;i<15;i++){
        printf("key:%d,rt:%d\n",i,deleteST(st,i));
   }
   temp_d=st->head;
   while(temp_d){
       temp_n=temp_d->next;
       while(temp_n){
           printf("[%d %ld] ",temp_n->key,(unsigned long)(temp_n->value));    
           temp_n=temp_n->next;
       }
       printf("\n");
       temp_d=temp_d->down;
   }
}
