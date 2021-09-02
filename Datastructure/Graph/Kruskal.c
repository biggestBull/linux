#include"./Kruskal.h"

static void *
_getValue(void * st){
   return ((kruskal_st *)st)->C;        
}

static graphinfo_st *
_initGraph(int size){
   int i;
   graphinfo_st * graph;

   if((graph=malloc(sizeof(graphinfo_st)*size))==NULL){
       return NULL;
   }
   for(i=0;i<size;i++){
       (graph+i)->father=-1;
   }
   return graph;
}

static void
_reverseFather(graphinfo_st * graph,int old_father,int new_father,int distance){
   if((graph+old_father)->father!=-1){
       _reverseFather(graph,(graph+old_father)->father,old_father,(graph+old_father)->distance);
   }
   (graph+old_father)->father=new_father;
   (graph+old_father)->distance=distance;
}

graphinfo_st *
kruskal(kruskal_st * ks,int size,int vnum){
   graphinfo_st * graph;
   int * djs,U,V;
   struct Heap * heap;
   kruskal_st * temp;

   if((djs=DJSetInit(size))==NULL)
       return NULL;
   if((heap=create_Heap(INT,0,_getValue))==NULL){
       free(djs);
       return NULL;
   }
   if(construct_Heap_fromArray(heap,ks,size)!=0){
       release_heap(heap);
       free(djs);
       return NULL;
   }
   if((graph=_initGraph(vnum))==NULL){
       release_heap(heap);
       free(djs);
       return NULL;
   }
   while(heap_pop(heap,&temp)==0){
       U=DJFind(djs,temp->v1);     
       V=DJFind(djs,temp->v2);
       if(U!=V){
           DJUnion(djs,U,V);
           if(graph[temp->v2].father==-1){
               graph[temp->v2].father=temp->v1;
               graph[temp->v2].distance=temp->C;
           }else if(graph[temp->v1].father==-1){
               graph[temp->v1].father=temp->v2;     
               graph[temp->v1].distance=temp->C;
           }else{
               printf("zao:U:%d,V:%d\n",temp->v1,temp->v2);
               _reverseFather(graph,temp->v1,temp->v2,temp->C);
           }
       }
   }
   free(djs);
   release_heap(heap);
   return graph;
}










