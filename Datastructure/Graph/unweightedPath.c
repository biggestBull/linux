#include"./unweightedPath.h"

static graphinfo_st *
_initGraphInfo(int size,struct Queue * queue,int v,ljb_st ** ljbs){
   graphinfo_st *  graphinfo;

   if((graphinfo=malloc(sizeof(graphinfo_st)*size))==NULL)
       return NULL;
   if(create_queue(queue,PTR)!=0)
       return NULL;
   for(size--;size>=0;size--){
       graphinfo[size].distance=(unsigned int)(-1)/2;
       graphinfo[size].father=-1;
       graphinfo[size].known=0;
   }
   graphinfo[v].distance=0;
   queue_push(queue,ljbs[v]);
   return graphinfo;
}

graphinfo_st *
unweightedPath(ljb_st ** ljbs,int size,int v){
   graphinfo_st * graphinfo;
   struct Queue queue;
   int cur_dis,cur_v;
   ljb_st * temp;

   if((graphinfo=_initGraphInfo(size,&queue,v,ljbs))==NULL)
       return NULL;
   while(queue_pop(&queue,&temp)!=-1){
        cur_v=temp->v;
        graphinfo[cur_v].known=1;
        cur_dis=graphinfo[cur_v].distance;
        while(temp->next){
           temp=temp->next;
           if(!graphinfo[temp->v].known){
               if(graphinfo[temp->v].distance>cur_dis+1){
                   graphinfo[temp->v].father=cur_v;
                   graphinfo[temp->v].distance=cur_dis+1;
               } 
               if(queue_push(&queue,ljbs[temp->v])==-1){
                   queue_release(&queue);
                   free(graphinfo);
                   return NULL;
               }
           }
        }
   }
   queue_release(&queue);
   return graphinfo;
}

