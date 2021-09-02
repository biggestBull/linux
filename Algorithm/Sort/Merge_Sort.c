#include"./sort.h"


static void
_merge(int * arr,int * arr_b,int s,int e,int flag){
   int i,j,k,mid;
   if(e-s<1)
       return;
   mid=(s+e)/2;
   _merge(arr,arr_b,s,mid,flag);
   _merge(arr,arr_b,mid+1,e,flag);
   
   i=s,j=mid+1,k=0;
   while(i<=mid && j<=e){
       if(flag){
           if(arr[i]>arr[j])
               arr_b[k++]=arr[i++];
           else
               arr_b[k++]=arr[j++];
       }else{
           if(arr[i]<arr[j])
               arr_b[k++]=arr[i++];
           else
               arr_b[k++]=arr[j++];
                
       }
   }
   while(i<=mid)
       arr_b[k++]=arr[i++];
   while(j<=e)
       arr_b[k++]=arr[j++];
   for(i=0;i<k;i++){
       arr[s++]=arr_b[i];     
   }

}

int
merge_sort(int * arr,int length,int flag){
   int * arr_b;
   if((arr_b=malloc(sizeof(int)*length))==NULL)
       return -1;
   _merge(arr,arr_b,0,length-1,flag);
   free(arr_b);
   return 0;
}
