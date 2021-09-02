#include<stdio.h>
#include"./ConstructLJB.h"
#include"./Dijkstra.h"


typedef enum{
   V1,V2,V3,V4,V5,V6,V7        
}subject_tp;

static void
constructLJBS(ljb_st ** ljbs){
   ljbs[0]=constructLJB(NULL,V1,1);
   constructLJB(ljbs[0],V4,1);
   constructLJB(ljbs[0],V2,2);
   
   ljbs[1]=constructLJB(NULL,V2,1);
   constructLJB(ljbs[1],V4,3);
   constructLJB(ljbs[1],V5,10);
   
   ljbs[2]=constructLJB(NULL,V3,1);
   constructLJB(ljbs[2],V1,4);
   constructLJB(ljbs[2],V6,5);
   
   ljbs[3]=constructLJB(NULL,V4,1);
   constructLJB(ljbs[3],V3,2);
   constructLJB(ljbs[3],V5,2);
   constructLJB(ljbs[3],V6,8);
   constructLJB(ljbs[3],V7,4);
   
   
   ljbs[4]=constructLJB(NULL,V5,1);
   constructLJB(ljbs[4],V7,6);
   
   ljbs[5]=constructLJB(NULL,V6,1);
   
   ljbs[6]=constructLJB(NULL,V7,1);
   constructLJB(ljbs[6],V6,1);


}
int
main(){
   ljb_st *ljbs[7];      
   graphinfo_st * graphinfo;
   int i;

   constructLJBS(ljbs);
   graphinfo=dijkstra(ljbs,7,0);
   for(i=0;i<7;i++){
       switch(i){

           case V1:
               printf("V1:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case V2:
               printf("V2:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case V3:
               printf("V3:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case V4:
               printf("V4:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case V5:
               printf("V5:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case V6:
               printf("V6:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case V7:
               printf("V7:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
       }
   }
}

