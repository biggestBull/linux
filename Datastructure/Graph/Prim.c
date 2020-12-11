

#include"./Prim.h"

static void *
_getValue(void * st){
   return ((ljb_st *)(st))->C;        
}


static int
_init(struct Heap ** heap,graphinfo_st ** graph,ljb_st ** ljbs,int size ,int v){
   int i;

   if((*heap=create_Heap(INT,0,_getValue))==NULL)
       return -1;
   if((*graph=malloc(sizeof(graphinfo_st)*size))==NULL){
       release_heap(*heap);     
       return -1;
   }
   for(i=0;i<size;i++){
       ((*graph)+i)->known=0;
       ((*graph)+i)->father=-1;
       ((*graph)+i)->distance=_INT_MAX;
   }
   ((*graph)+v)->distance=0;
   if(heap_insert(*heap,ljbs[v])!=0){
       free(*graph);
       release_heap(*heap);
       *graph=NULL;
       *heap=NULL;
       return -1;
   }
   return 0;
}

graphinfo_st * 
prim(ljb_st ** ljbs,int size,int v){
   struct Heap *heap;
   graphinfo_st * graph;
   ljb_st * temp;
   int cur_f;

   if(_init(&heap,&graph,ljbs,size,v)==-1)
       return NULL;
   while(heap_pop(heap,&temp)==0){
       cur_f=temp->v;
       if(graph[cur_f].known)
           continue;
       graph[cur_f].known=1;
       while(temp->next){
           temp=temp->next;
           if(!graph[temp->v].known){
               if(temp->C<graph[temp->v].distance){
                  graph[temp->v].distance=temp->C;
                  graph[temp->v].father=cur_f;
                  if(heap_insert(heap,ljbs[temp->v])!=0){
                     release_heap(heap);
                     free(graph);
                     return NULL;
                  }
               }
           }
       }

   }

   release_heap(heap);
   return graph;
}













