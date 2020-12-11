#include"./ConstructLJB.h"

ljb_st *
constructLJB(ljb_st * ljb,int v,int C){
   if(ljb){
      ljb_st * temp;
      while(ljb->next){
         ljb=ljb->next;    
      }
      temp=malloc(sizeof(ljb_st));
      ljb->next=temp;
      ljb=temp;
   }else{
      ljb=malloc(sizeof(ljb_st));
   }
   ljb->v=v;
   ljb->C=C;
   ljb->next=NULL;

   return ljb;
}

