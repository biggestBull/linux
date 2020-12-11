#include"./FindArtP.h"

#include<stdio.h>
typedef struct{
   int known;
   int low;
   int num;
   int parent;
}findartp_st;

static void
_find(ljb_st ** ljbs,int v,findartp_st * fdp,int * rt_arr,int * count){
   ljb_st * temp;
   fdp[v].num=fdp[v].low=++(*count);
   fdp[v].known=1;
   temp=ljbs[v];
   while(temp->next){
       temp=temp->next;
       if(!(fdp+temp->v)->known){
           fdp[temp->v].parent=v;
           _find(ljbs,temp->v,fdp,rt_arr,count);
           if(fdp[temp->v].low>=fdp[v].num)
               rt_arr[v]=1;
           fdp[v].low=fdp[v].low<fdp[temp->v].low?fdp[v].low:fdp[temp->v].low;
           
       }else{
           if(fdp[v].parent!=temp->v)
               fdp[v].low=fdp[temp->v].num<fdp[v].low?fdp[temp->v].num:fdp[v].low;
       }
   }
}


int *
findartp(ljb_st ** ljbs,int size){
   int * rt_arr,index=0,count=0;
   findartp_st * fdp;
   ljb_st * temp;

   if((rt_arr=malloc(sizeof(int)*size))==NULL)
       return NULL;
   memset(rt_arr,0,sizeof(int)*size);
   if((fdp=malloc(sizeof(findartp_st)*size))==NULL){
       free(rt_arr);
       return NULL;
   }
   for(index=size-1;index>=0;index--){
       fdp[index].known=0;
   }
   _find(ljbs,0,fdp,rt_arr,&count);
   for(index=0;index<size;index++){
       if(index==0 && rt_arr[0]==1){
           rt_arr[index]=0;
           temp=ljbs[0];
           count=fdp[0].low;
           if(temp->next && temp->next->next)
               while(temp->next){
                   temp=temp->next;     
                   if(count!=fdp[temp->v].low){
                       rt_arr[index]=1;
                       break;
                   }
               }
       }
   }
   return rt_arr;
}
























