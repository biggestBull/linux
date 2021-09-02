#include"./DisjointSet.h"

int * 
DJSetInit(int size){
   int * set;

   if((set=malloc(sizeof(int)*size))==NULL)
       return NULL;
   for(size--;size>=0;size--)
       set[size]=-1;
   return set;
}

int
DJFind(int * DJSet,int X){
   if(DJSet[X]<0)
       return X;
   else
       return DJSet[X]=DJFind(DJSet,DJSet[X]);
}

void
DJUnion(int * DJSet,int X,int Y){
   int U,V;
   U=DJFind(DJSet,X);
   V=DJFind(DJSet,Y);

   if(DJSet[U]<DJSet[V]){
      DJSet[U]+=DJSet[V];
      DJSet[V]=U;
   }else{
      DJSet[V]+=DJSet[U];
      DJSet[U]=V;
   }
}
