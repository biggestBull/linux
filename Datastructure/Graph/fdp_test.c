
#include<stdio.h>
#include"./ConstructLJB.h"
#include"./FindArtP.h"


typedef enum{
   V1,V2,V3,V4,V5,V6,V7        
}subject_tp;

static void
constructLJBS(ljb_st ** ljbs){
   ljbs[0]=constructLJB(NULL,V1,1);
   constructLJB(ljbs[0],V4,1);
   constructLJB(ljbs[0],V2,1);
   
   ljbs[1]=constructLJB(NULL,V2,1);
   constructLJB(ljbs[1],V4,1);
   constructLJB(ljbs[1],V1,1);
   
   ljbs[2]=constructLJB(NULL,V3,1);
   constructLJB(ljbs[2],V4,1);
   constructLJB(ljbs[2],V6,1);
   
   ljbs[3]=constructLJB(NULL,V4,1);
   constructLJB(ljbs[3],V1,1);
   constructLJB(ljbs[3],V2,1);
   constructLJB(ljbs[3],V3,1);
   constructLJB(ljbs[3],V5,1);
   
   
   ljbs[4]=constructLJB(NULL,V5,1);
   constructLJB(ljbs[4],V4,1);
   constructLJB(ljbs[4],V7,1);
   
   ljbs[5]=constructLJB(NULL,V6,1);
   constructLJB(ljbs[5],V3,1);
   constructLJB(ljbs[5],V7,1);
   
   ljbs[6]=constructLJB(NULL,V7,1);
   constructLJB(ljbs[6],V5,1);
   constructLJB(ljbs[6],V6,1);


}
int
main(){
   ljb_st *ljbs[7];      
   int i,*rt_arr;

   constructLJBS(ljbs);
   rt_arr=findartp(ljbs,7);
   for(i=0;i<7;i++){
       if(rt_arr[i]==1)
           printf("V%d ",i+1);
   }
   printf("\n");
}

