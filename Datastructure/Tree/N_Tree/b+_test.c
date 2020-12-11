#include<stdio.h>
#include"./B+Tree.h"

struct test{
   int a;
   char c;
};

void
print(int fd,struct B43Tree_Node * bn,off_t offset){
   int i;
   struct B43Tree_Node son;

   lseek(fd,offset,SEEK_SET);
      for(i=0;i<bn->cur_key_num;i++){
         printf("key:%d ",bn->sons[i].key);      
   if(!bn->isleave){
         read(fd,&son,sizeof(struct B43Tree_Node));
         print(fd,&son,son.sons[i].ptr);
      }

   }
       printf("\n");
}


int
main(){
   struct B43Tree * BT;
   struct test test;
   int i,ptrs[30],j=0,z=0,k;
   struct B43Tree_Node BN;
   test.a=1;
   test.c='a';

   printf("size:%d,%p\n",sizeof(struct B43Tree_Node),createB43Tree("./temp",sizeof(struct test)));
   printf("%p\n",BT=openB43Tree("./temp"));
   printf("BT->Root:%d\n",BT->offset);
   //BT->offset=238;
   for(i=0;i<15  ;i++){
       test.a=i;
       test.c='a'+i;
       printf("%d\n",B43Tinsert(BT,i,&test));
   }
   
 /*  for(i=41;i<45;i++){
       test.a=i;
       test.c='a'+i;
       printf("%d\n",B43Tinsert(BT,i,&test));
   }
   for(i=51;i<60;i++){
       test.a=i;
       test.c='a'+i;
       printf("%d\n",B43Tinsert(BT,i,&test));
   }
   
   printf("after insert:cur:%d\n",BT->Root.cur_key_num);
   for(i=0;i<60;i++){
       printf("%d,%c,%d\n",test.a,test.c,B43Tsearch(BT,i,&test));     
   }
   printf("\nBT_>Root:%d\n",BT->offset);
   for(i=0;i<BT->Root.cur_key_num;i++){
       printf("parent:%d,key:%d,ptr:%d\n",BT->Root.parent,BT->Root.sons[i].key,BT->Root.sons[i].ptr);     
       ptrs[j++]=BT->Root.sons[i].ptr;
   }
   ptrs[j++]=BT->Root.other_son;
   printf("\nsons:\n");
   for(i=0;i<j;i++){
       lseek(BT->fd,ptrs[i],SEEK_SET);
       read(BT->fd,&BN,sizeof(struct B43Tree_Node));
       printf("son%d,parent:%d\n",i,BN.parent);
       for(k=0;k<BN.cur_key_num;k++){
          printf("key:%d,ptr:%d\n",BN.sons[k].key,BN.sons[k].ptr);    
          ptrs[10+z++]=BN.sons[k].ptr;
       }
       ptrs[10+z++]=BN.other_son;
   }
   printf("grandsons\n");
   for(i=0;i<z;i++){
       lseek(BT->fd,ptrs[i+10],SEEK_SET);
       read(BT->fd,&BN,sizeof(struct B43Tree_Node));
       printf("grandson%d,parent:%d\n",i,BN.parent);
       for(k=0;k<BN.cur_key_num;k++){
          printf("key:%d,ptr:%d\n",BN.sons[k].key,BN.sons[k].ptr);    
       }
   }
   printf("start delete\n");
   */for(i=10;i>2;i--){
       printf("del:%d,key:%d\n",B43Tdelete(BT,i),i);
   }
   
 /*  for(i=5;i>2;i--){
       printf("del:%d,key:%d\n",B43Tdelete(BT,i),i);
   }*/
   printf("after delete:cur:%d\n",BT->Root.cur_key_num);
   for(i=0;i<15;i++){
       printf("%d,%c,%d\n",test.a,test.c,B43Tsearch(BT,i,&test));     
   }
  /* printf("\nBT_>Root:%d\n",BT->offset);
   for(i=0;i<BT->Root.cur_key_num;i++){
       printf("parent:%d,key:%d,ptr:%d\n",BT->Root.parent,BT->Root.sons[i].key,BT->Root.sons[i].ptr);     
       ptrs[j++]=BT->Root.sons[i].ptr;
   }
   ptrs[j++]=BT->Root.other_son;
   printf("\nsons:\n");
   for(i=0;i<j;i++){
       lseek(BT->fd,ptrs[i],SEEK_SET);
       read(BT->fd,&BN,sizeof(struct B43Tree_Node));
       printf("son%d,parent:%d\n",i,BN.parent);
       for(k=0;k<BN.cur_key_num;k++){
          printf("key:%d,ptr:%d\n",BN.sons[k].key,BN.sons[k].ptr);     
       }
   }
*/
   }
