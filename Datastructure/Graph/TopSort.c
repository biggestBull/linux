#include"./TopSort.h"

static int 
_initQueue(ljb_st ** ljbs,struct Queue * queue,int * indegree,int size){
    
    if(create_queue(queue,PTR)!=0)
        return -1;
    for(size--;size>=0;size--){
       if(indegree[size]==0){
           if(queue_push(queue,ljbs[size])==-1)
               return -1;     
       }
    }
    return 0;
}

int *
topSort(ljb_st ** ljbs,int * indegree,int size){
    struct Queue  queue;
    ljb_st * temp;
    int i=0,*rt_arr;

    if(_initQueue(ljbs,&queue,indegree,size)!=0)
        return NULL;
    rt_arr=malloc(sizeof(int)*size);
    while(queue_pop(&queue,&temp)!=-1){
        rt_arr[i++]=temp->v;
        while(temp->next){
           temp=temp->next;
           indegree[temp->v]--;
           if(indegree[temp->v]==0){
               if(queue_push(&queue,ljbs[temp->v])==-1){
                   free(rt_arr);   
                   queue_release(&queue);
                   return NULL;
               }
           }
        }
    }
    queue_release(&queue);
    if(i!=size){
       free(rt_arr);
       return NULL;
    }
    return rt_arr;
}

