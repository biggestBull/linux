#include<stdio.h>
#include"./arrayTree.h"

int 
main(){
   array_tree_sum_st * ats;
   int i,nums[]={
       1,1,1,1,1,1,1,1,1,1     
   };
   ats=initAT(nums,10);
   for(i=0;i<=10;i++){
       printf("[%d,%d] ",ats->Sum1[i],ats->Sum2[i]);     
   }
   printf("\n\n");
   printf("[1,3] %d\n",getSumAT(ats,1,3));
   printf("[4,9] %d\n",getSumAT(ats,4,9));
   printf("[1,10] %d\n",getSumAT(ats,1,10));

   printf("update[4,5,1] ");
   updateAT(ats,4,5,1);
   printf("update[7,9,3] ");
   updateAT(ats,7,9,3);
   printf("update[1,2,-1] ");
   updateAT(ats,1,2,-1);

   printf("update[6,6,2] ");
   updateAT(ats,6,6,2);
   printf("\n");
   printf("[1,3] %d\n",getSumAT(ats,1,3));
   printf("[4,9] %d\n",getSumAT(ats,4,9));
   printf("[1,10] %d\n",getSumAT(ats,1,10));
}
