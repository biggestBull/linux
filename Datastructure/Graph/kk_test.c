#include<stdio.h>
#include"./Kruskal.h"

typedef enum{
   V1,V2,V3,V4,V5,V6,V7        
}subject_tp;

static void
_c(kruskal_st * kk,int v1,int v2,int C){
   kk->v1=v1;
   kk->v2=v2;
   kk->C=C;
}

static void
constructLJBS(kruskal_st * kk,kruskal_st ** _kk){
  int i;
   _c(kk,V1,V4,1);
   _c(kk+6,V4,V7,4);
   _c(kk+4,V2,V4,3);
   _c(kk+2,V1,V2,2);
   _c(kk+8,V7,V5,6);
   _c(kk+1,V6,V7,1);
   _c(kk+5,V1,V3,4);
   _c(kk+3,V4,V3,2);
   _c(kk+7,V3,V6,5);

/*   _c(kk,V1,V2,2);
   _c(kk+1,V4,V1,1);
   _c(kk+2,V1,V3,2);
   _c(kk+3,V2,V3,1);
   _c(kk,V4,V3,1);
*/
   for(i=0;i<9;i++){
      _kk[i]=kk+i;
   }

}
int
main(){
   kruskal_st  kk[9],*_kk[9];      
   graphinfo_st * graphinfo;
   int i;

   constructLJBS(kk,_kk);
   graphinfo=kruskal(_kk,9,7);
   for(i=0;i<7;i++){
       switch(i){

           case V1:
               printf("V1:distance:%d,father:V%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father+1,graphinfo[i].known);
               break;
           case V2:
               printf("V2:distance:%d,father:V%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father+1,graphinfo[i].known);
               break;
           case V3:
               printf("V3:distance:%d,father:V%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father+1,graphinfo[i].known);
               break;
           case V4:
               printf("V4:distance:%d,father:V%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father+1,graphinfo[i].known);
               break;
           case V5:
               printf("V5:distance:%d,father:V%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father+1,graphinfo[i].known);
               break;
           case V6:
               printf("V6:distance:%d,father:V%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father+1,graphinfo[i].known);
               break;
           case V7:
               printf("V7:distance:%d,father:V%d,known:%d\n ",graphinfo[i].distance,graphinfo[i].father+1,graphinfo[i].known);
       }
   }
}


