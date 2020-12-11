#include"./sort.h"

static void
swap(int * a,int * b){
   int temp;
   temp=*a;
   *a=*b;
   *b=temp;
}

static void
percDown(int * arr,int length,int index,int flag){
   int i,j,temp;

   temp=arr[index];
   while((2*index)+1<length){
       i=2*index+1;

       if(flag){
           if(i+1<length && arr[i]>arr[i+1]){
               i++;     
           }
           if(temp>arr[i]){
               arr[index]=arr[i];
           }else
               break;
       }else{
           if(i+1<length && arr[i]<arr[i+1]){
               i++;     
           }
           if(temp<arr[i]){
               arr[index]=arr[i];
           }else
               break;
                
       }
       index=i;
   }
   arr[index]=temp;
}

void
heap_sort(int * arr,int length,int flag){
   int i;

   for(i=length/2;i>=0;i--){
       percDown(arr,length,i,flag);     
   }
   for(i=length-1;i>0;i--){
       swap(arr,arr+i);
       percDown(arr,i,0,flag);     
   }
}
