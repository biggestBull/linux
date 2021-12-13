#include"./SplayTree.h"


int
main(int argc,char * argv[]){
   struct SplayTree_ST * SplayT,*temp_p;
   long temp;

   printf("create:[key:%d,value:%d,return:%p]\n",1,1,SplayT=create_SplayT(1,LONG));
   printf("insert:[key:%d,value:%d,return:%d]\n",1,2,temp_p=insert_SplayT(SplayT,1,2));
   printf("search:[key:%d,value:%d,return:%d]\n",1,temp,search_SplayT(SplayT,1,&temp));
   
   printf("insert:[key:%d,value:%d,return:%d]\n",4,3,temp_p=insert_SplayT(SplayT,4,3));
   printf("insert:[key:%d,value:%d,return:%d]\n",4,19,temp_p=insert_SplayT(SplayT,4,19));
   printf("insert:[key:%d,value:%d,return:%d]\n",2,0,temp_p=insert_SplayT(SplayT,2,0));
   printf("insert:[key:%d,value:%d,return:%d]\n",5,6,temp_p=insert_SplayT(SplayT,5,6));
   printf("insert:[key:%d,value:%d,return:%d]\n",10,2,temp_p=insert_SplayT(SplayT,10,2));
   printf("insert:[key:%d,value:%d,return:%d]\n",7,2,temp_p=insert_SplayT(SplayT,7,2));

   printf("search:[key:%d,value:%d,return:%d]\n",4,temp,search_SplayT(SplayT,4,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",3,temp,search_SplayT(SplayT,3,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",2,temp,search_SplayT(SplayT,2,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",5,temp,search_SplayT(SplayT,5,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",10,temp,search_SplayT(SplayT,10,&temp));
   printf("search:[key:%d,value:%d,return:%d]\n",7,temp,search_SplayT(SplayT,7,&temp));
   
   printf("delete:[key:%d,value:%d,return:%d]\n",5,2,temp_p=delete_SplayT(SplayT,5));
   printf("delete:[key:%d,value:%d,return:%d]\n",5,2,temp_p=delete_SplayT(SplayT,5));
  
   printf("search:[key:%d,value:%d,return:%d]\n",5,temp,search_SplayT(SplayT,5,&temp));
   
   printf("insert:[key:%d,value:%d,return:%d]\n",5,2,temp_p=insert_SplayT(SplayT,5,2));
   printf("insert:[key:%d,value:%d,return:%d]\n",10,2,temp_p=insert_SplayT(SplayT,10,2));

   free_SplayT(SplayT);
}
