#include"./sort.h"
#include<stdio.h>

int
main(int argc,char * argv[]){
   int test[]={
       2421,53463,6474,24,5757,23425,758,3425,768,32,4657,2335,657,34235,-435,76,435,-2,354,-24325,12356,-1,0,-97,3     
   },i,length;

   length=sizeof(test)/sizeof(int);
   for(i=0;i<length;i++){
       printf("%d ",test[i]);     
   }
   printf("\n\n");
   merge_sort(test,length,1);
   for(i=0;i<length;i++){
       printf("%d ",test[i]);     
   }
   printf("\n\n");
   merge_sort(test,length,1);
   for(i=0;i<length;i++){
       printf("%d ",test[i]);     
   }
   printf("\n");
}
