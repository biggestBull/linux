#include"./sort.h"

void
shell_sort(int * arr,int length,int flag){
   int i,j,increment,temp;

   if(length<2)
       return;

   for(increment=length/2;increment>0;increment/=2){
       i=increment;
       for(;i<length;i++){
           temp=arr[i];
           for(j=i;j>=increment;j-=increment){
               if(( flag && temp>arr[j-increment])|| (!flag && temp<arr[j-increment]))
                   arr[j]=arr[j-increment];
               else
                   break;
           }
           arr[j]=temp;
       }
   }
    return ;        
}

