#include"./TopSort.h"
#include<stdio.h>


typedef enum{
   JAVA,HTML,JSP,SERVLET,SSM,SPRINGBOOT        
}subject_tp;

static void
constructLJBS(ljb_st ** ljbs,int * indegree){
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

   indegree[JAVA]=0;
   indegree[HTML]=0;
   indegree[SERVLET]=1;
   indegree[JSP]=2;
   indegree[SSM]=1;
   indegree[SPRINGBOOT]=1;
}
int
main(){
   ljb_st *ljbs[6];      
   int indegree[6],*toptb,i;

   constructLJBS(ljbs,indegree);
   toptb=topSort(ljbs,indegree,6);
   for(i=0;i<6;i++){
       switch(toptb[i]){
           case JAVA:
               printf("JAVA->");
               break;
           case HTML:
               printf("HTML->");
               break;
           case SERVLET:
               printf("SERVLET->");
               break;
           case JSP:
               printf("JSP->");
               break;
           case SSM:
               printf("SSM->");
               break;
           case SPRINGBOOT:
               printf("SPRINGBOOT->");
       }
   }
   fflush(stdout);
   printf("\b\b\n");

}
