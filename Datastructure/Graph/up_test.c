#include<stdio.h>
#include"./ConstructLJB.h"
#include"./unweightedPath.h"
#include<stdio.h>


typedef enum{
   JAVA,HTML,JSP,SERVLET,SSM,SPRINGBOOT        
}subject_tp;

static void
constructLJBS(ljb_st ** ljbs){
   ljbs[0]=constructLJB(NULL,JAVA,1);
   constructLJB(ljbs[0],JSP,1);
   
   ljbs[1]=constructLJB(NULL,HTML,1);
   constructLJB(ljbs[1],JSP,1);
   
   ljbs[2]=constructLJB(NULL,JSP,1);
   constructLJB(ljbs[2],SERVLET,1);
   
   ljbs[3]=constructLJB(NULL,SERVLET,1);
   constructLJB(ljbs[3],SSM,1);
   
   
   ljbs[4]=constructLJB(NULL,SSM,1);
   constructLJB(ljbs[4],SPRINGBOOT,1);
   
   ljbs[5]=constructLJB(NULL,SPRINGBOOT,1);
   constructLJB(ljbs[5],JAVA,1);
   constructLJB(ljbs[5],HTML,1);


}
int
main(){
   ljb_st *ljbs[6];      
   graphinfo_st * graphinfo;
   int i;

   constructLJBS(ljbs);
   graphinfo=unweightedPath(ljbs,6,SPRINGBOOT);
   for(i=0;i<6;i++){
       switch(i){

           case JAVA:
               printf("JAVA:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case HTML:
               printf("HTML:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case SERVLET:
               printf("SERVLET:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case JSP:
               printf("JSP:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case SSM:
               printf("SSM:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
               break;
           case SPRINGBOOT:
               printf("SPRINGBOOT:distance:%d,father:%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father,graphinfo[i].known);
       }
   }
   printf("\n");
}
