#include"./sort.h"

static void
insert_sort(int * arr,int left,int right,int flag){
   int i,j,temp;

   for(i=left+1;i<=right;i++){
       temp=arr[i];
       for(j=i-1;j>=left && ((flag && temp>arr[j]) || (!flag && temp<arr[j]));j--){
           arr[j+1]=arr[j];
       }
       arr[j+1]=temp;
   }
}

static void
swap(int * a,int * b){
   int temp;
   temp=*a;
   *a=*b;
   *b=temp;
}

static int
medium(int * arr,int left,int right,int flag){
   int mid,privot;


   mid=(left+right)/2;
   if(arr[left]>arr[mid])
      swap(arr+left,arr+mid);
   if(arr[left]>arr[right])
       swap(arr+left,arr+right);
   if(arr[mid]>arr[right])
       swap(arr+mid,arr+right);
   
   if(flag){
       swap(arr+left,arr+right);
       swap(arr+mid,arr+left+1);
   }else
       swap(arr+mid,arr+right-1);
   privot=flag?arr[left+1]:arr[right-1];
   return privot;
}

void
quick_sort(int * arr,int left,int right,int flag){
   int privot,i,j;

   if(right-left>20){
       privot=medium(arr,left,right,flag);
       if(flag){
          i=left+1;
          j=right;
       }else{
           i=left;
           j=right-1;
       }
       while(1){
           if(flag){
              while(arr[++i]>privot) ;
              while(arr[--j]<privot) ;
              if(j>i)
                  swap(arr+i,arr+j);
              else
                  break;
           }else{
              while(arr[++i]<privot) ;
              while(arr[--j]>privot) ;
              if(j>i)
                  swap(arr+i,arr+j);
              else
                  break;
                    
           }
       }
       if(!flag){
         swap(arr+i,arr+right-1);
         quick_sort(arr,left,i-1,flag);
         quick_sort(arr,i+1,right,flag);
       }else{
         swap(arr+j,arr+left+1);
         quick_sort(arr,left,j-1,flag);
         quick_sort(arr,j+1,right,flag);
                
       }
                
   }else{
       insert_sort(arr,left,right,flag);     
   }
}
