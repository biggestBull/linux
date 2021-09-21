#include"./AVLTree.h"
#include<stdio.h>

int
main(int argc,char * argv[]){
   struct AVLTree_ST * AVLT;
   struct AVLTree_Node * temp_p;
   long temp;

   printf("create AVLTree : LONG \n",AVLT=create_AVLT(LONG));
   printf("insert:[key:%d,value:%d,return:%d]\n",1,2,insert_AVLT(AVLT,1,2));
   printf("delete:[key:%d,value:%d,return:%d]\n",1,2,temp_p=delete_AVLT(AVLT,1));
   printf("search:[key:%d,value:%d,return:%d]\n",1,temp,search_AVLT(AVLT,1,&temp));
   
   printf("insert:[key:%d,value:%d,return:%d]\n",4,3,temp_p=insert_AVLT(AVLT,4,3));
   printf("insert:[key:%d,value:%d,return:%d]\n",3,19,temp_p=insert_AVLT(AVLT,3,19));
   printf("insert:[key:%d,value:%d,return:%d]\n",2,0,temp_p=insert_AVLT(AVLT,2,0));
   printf("insert:[key:%d,value:%d,return:%d]\n",5,6,temp_p=insert_AVLT(AVLT,5,6));
   printf("insert:[key:%d,value:%d,return:%d]\n",10,2,temp_p=insert_AVLT(AVLT,10,2));
   printf("insert:[key:%d,value:%d,return:%d]\n",7,2,temp_p=insert_AVLT(AVLT,7,2));

   printf("search:[key:%d,value:%d,return:%d]\n",11,temp,search_AVLT(AVLT,11,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",3,temp,search_AVLT(AVLT,3,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",2,temp,search_AVLT(AVLT,2,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",5,temp,search_AVLT(AVLT,5,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",10,temp,search_AVLT(AVLT,10,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",7,temp,search_AVLT(AVLT,7,&temp));
   
   printf("delete:[key:%d,value:%d,return:%d]\n",5,2,temp_p=delete_AVLT(AVLT,5));
   printf("delete:[key:%d,value:%d,return:%d]\n",5,2,temp_p=delete_AVLT(AVLT,5));
  
   printf("search:[key:%d,value:%d,return:%d]\n",5,temp,search_AVLT(AVLT,5,&temp));
   
   printf("insert:[key:%d,value:%d,return:%d]\n",5,2,temp_p=insert_AVLT(AVLT,5,2));
   printf("insert:[key:%d,value:%d,return:%d]\n",10,2,temp_p=insert_AVLT(AVLT,10,2));

   free_AVLT(AVLT);
}
