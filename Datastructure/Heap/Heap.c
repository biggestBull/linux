#include"./Heap.h"

#define HEAP_INIT_SIZE 100
#define HEAP_MAX_SIZE 10000

static int 
_judge(enum _TYPE type,void * a,void * b){
   switch(type){
       case CHAR:
          return  (char)a>(char)b?1:-1;
       case INT:
          return (int )a>(int )b?1:-1;
       case LONG:
          return (long )a>(long )b?1:-1;
   }
   return -1;
}

struct Heap *
create_Heap(enum _TYPE heap_type,int flag,void * (*func)(void *)){
   struct Heap * heap;

   if((heap=malloc(sizeof(struct Heap)))==NULL)
       return NULL;
   if((heap->arr=malloc(sizeof(unsigned long)*HEAP_INIT_SIZE))==NULL){
       free(heap);
       return NULL;
   }
   heap->type=heap_type;
   heap->size=HEAP_INIT_SIZE;
   heap->cur_size=0;
   heap->flag=flag;
   heap->func=func;
   switch(heap_type){
       case CHAR:
           *(heap->arr)=flag?(unsigned char)(-1)/2:1<<sizeof(char)*8-1;
           break;
       case INT:
           *(heap->arr)=flag?(unsigned int)(-1)/2:1<<sizeof(int)*8-1;
           break;
       case LONG:
           *(heap->arr)=flag?(unsigned long)(-1)/2:1L<<sizeof(long)*8-1; 
		   //字面量默认是int，为了避免溢出，应当加上L标志它是一个long型
           break;
       default:
           return NULL;

   }

   return heap;
}

int
construct_Heap_fromArray(struct Heap * heap,void * arr,int arr_len){
  int i,j;
  void * temp_addr;

  while(arr_len>=heap->size){
      if(heap->size*2>=HEAP_MAX_SIZE)
          return 1;
      if((temp_addr=malloc(sizeof(unsigned long)*heap->size*2))==NULL)  //用一临时变量来接收，因为还要释放前面的 
          return -1;
      free(heap->arr);
      heap->arr=temp_addr;
      heap->size*=2;
  }
  if(heap->func==NULL){
      switch(heap->type){
          case CHAR:
              for(i=0;i<arr_len;i++){
                 char c;
                 c=*(char *)(arr+i);
                 heap->cur_size++;
                 if(heap->flag){                          
                    for(j=heap->cur_size;c>(char)(heap->arr[j/2]);j/=2){
                       heap->arr[j]=heap->arr[j/2];        
                    }
                 }else{
                    for(j=heap->cur_size;c<(char)(heap->arr[j/2]);j/=2){
                       heap->arr[j]=heap->arr[j/2];        
                    }
                 }
                 heap->arr[j]=c;
              }

              break;
          case INT:
              for(i=0;i<arr_len;i++){
                 int c;
                 c=*(int *)((int *)(arr)+i);
                 heap->cur_size++;
                 if(heap->flag){                          
                    for(j=heap->cur_size;c>(int)(heap->arr[j/2]);j/=2){
                       heap->arr[j]=heap->arr[j/2];        
                    }
                 }else{
                    for(j=heap->cur_size;c<(int)(heap->arr[j/2]);j/=2){
                       heap->arr[j]=heap->arr[j/2];        
                    }
                 }
                 heap->arr[j]=c;
              }
              break;
          case LONG:
              for(i=0;i<arr_len;i++){
                 long c;
                 c=*(long *)((long *)(arr)+i);
                 heap->cur_size++;
                 if(heap->flag){                          
                    for(j=heap->cur_size;c>(long )(heap->arr[j/2]);j/=2){
                       heap->arr[j]=heap->arr[j/2];        
                    }
                 }else{
                    for(j=heap->cur_size;c<(long)(heap->arr[j/2]);j/=2){
                       heap->arr[j]=heap->arr[j/2];        
                    }
                 }
                 heap->arr[j]=c;
              }
              break;
          default:
              return -1;

      }
  }else{
      int i,j;
      void * a,*b,*c;
      memcpy(heap->arr+1,arr,arr_len*sizeof(unsigned long));
      for(i=1;i<=arr_len;i++){
         j=i; 
         c=heap->arr[j];
         b=(*(heap->func))(c);
         if(heap->flag){
            while(1){
               if(j/2==0)
                   a=heap->arr[0];
               else
                   a=(*(heap->func))(heap->arr[j/2]);
               if(_judge(heap->type,a,b)<0){
                   
                   heap->arr[j]=heap->arr[j/2];
                   j/=2;
               }else
                   break;
            }
         }else{
            while(1){
               if(j/2==0)
                   a=heap->arr[0];
               else
                   a=(*(heap->func))(heap->arr[j/2]);
               if(_judge(heap->type,a,b)>0){
                   heap->arr[j]=heap->arr[j/2];
                   j/=2;
               }else
                   break;
            }
                  
         }
         heap->arr[j]=c;      
         heap->cur_size++;
      }
      
  }   
  return 0; 
    
}


int
heap_insert(struct Heap * heap,void * value){
    void * temp_addr;
    int i,j;

    if(heap->cur_size>=heap->size){
      if(heap->size*2>=HEAP_MAX_SIZE)
          return 1;
      if((temp_addr=malloc(sizeof(void *)*heap->size*2))==NULL)   //同一个变量不允许多次free和malloc，所以用一个局部变量
          return -1;
      memcpy(temp_addr,heap->arr,sizeof(void *)*(heap->cur_size+1));
      free(heap->arr);
      heap->arr=temp_addr;
      heap->size*=2;
  }
  heap->cur_size++;
  if(heap->func==NULL){
      switch(heap->type){
          case CHAR:
              if(heap->flag){                          
                 for(j=heap->cur_size;(char)value>(char)(heap->arr[j/2]);j/=2){
                    heap->arr[j]=heap->arr[j/2];        
                 }
              }else{
                 for(j=heap->cur_size;(char)value<(char)(heap->arr[j/2]);j/=2){
                    heap->arr[j]=heap->arr[j/2];        
                 }
              }
              heap->arr[j]=(char)value;
              break;
          case INT:
              if(heap->flag){                          
                 for(j=heap->cur_size;(int)value>(int)(heap->arr[j/2]);j/=2){
                    heap->arr[j]=heap->arr[j/2];        
                 }
              }else{
                 for(j=heap->cur_size;(int)value<(int)(heap->arr[j/2]);j/=2){
                    heap->arr[j]=heap->arr[j/2];        
                 }
              }
              heap->arr[j]=(int)value;
              break;
          case LONG:
              if(heap->flag){                          
                 for(j=heap->cur_size;(long)value>(long)(heap->arr[j/2]);j/=2){
                    heap->arr[j]=heap->arr[j/2];        
                 }
              }else{
                 for(j=heap->cur_size;(long)value<(long)(heap->arr[j/2]);j/=2){
                    heap->arr[j]=heap->arr[j/2];        
                 }
              }
              heap->arr[j]=(long)value;
              break;
          default:
              return -1;

      }
  }else{
      int i,j;
      void * a,*b,*c;
     
      j=heap->cur_size;
      b=(*(heap->func))(value);
      if(heap->flag){
          while(1){
             if(j/2==0)
                 a=heap->arr[0];
             else
                 a=(*(heap->func))(heap->arr[j/2]);
      
             if(_judge(heap->type,a,b)<0){
                 heap->arr[j]=heap->arr[j/2];
                 j/=2;
             }else
                 break;
          }
      }else{
          while(1){
             if(j/2==0)
                 a=heap->arr[0];
             else
                 a=(*(heap->func))(heap->arr[j/2]);
         
             if(_judge(heap->type,a,b)>0){
                 heap->arr[j]=heap->arr[j/2];
                 j/=2;
             }else
                 break;
          }
                  
     }
     heap->arr[j]=value;      
  }
      
  return 0;   
           
}

int
heap_pop(struct Heap * heap,void * rt_value){
   unsigned long * tail_value;
   int i=1;

   if(heap->cur_size==0)
       return 1;
   if(heap->func==NULL){
            
       switch(heap->type){
            case CHAR:
                *(char *)rt_value=(char)(heap->arr[1]);
                tail_value=heap->arr[heap->cur_size--];
                if(heap->cur_size==0)
                    return 0;
                while(i*2<=heap->cur_size){
                   int j=2*i;
                   if(heap->flag){
                      if(j<heap->cur_size){
                          if((char)(heap->arr[j])<(char)(heap->arr[j+1]))
                              j++;
                      }
                      if((char)(tail_value)<(char)(heap->arr[j])){
                          heap->arr[i]=heap->arr[j];
                          i=j;
                      }else
                          break;

                   }else{
                      if(j<heap->cur_size){
                          if((char)(heap->arr[j])>(char)(heap->arr[j+1]))
                              j++;
                      }
                      if((char)(tail_value)>(char)(heap->arr[j])){
                          heap->arr[i]=heap->arr[j];
                          i=j;
                      }else
                          break;
                            
                   }
                }
                heap->arr[i]=tail_value;
                break;
            case INT:
                *(int *)rt_value=(int)(heap->arr[1]);
                tail_value=heap->arr[heap->cur_size--];
                if(heap->cur_size==0)
                    return 0;
                while(i*2<=heap->cur_size){
                   int j=2*i;
                   if(heap->flag){
                      if(j<heap->cur_size){
                          if((int)(heap->arr[j])<(int)(heap->arr[j+1]))
                              j++;
                      }
                      if((int)(tail_value)<(int)(heap->arr[j])){
                          heap->arr[i]=heap->arr[j];
                          i=j;
                      }else
                          break;

                   }else{
                      if(j<heap->cur_size){
                          if((int)(heap->arr[j])>(int)(heap->arr[j+1]))
                              j++;
                      }
                      if((int)(tail_value)>(int)(heap->arr[j])){
                          heap->arr[i]=heap->arr[j];
                          i=j;
                      }else
                          break;
                            
                   }
                }
                heap->arr[i]=tail_value;
                break;
            case LONG:
                *(long *)rt_value=(long)(heap->arr[1]);
                tail_value=heap->arr[heap->cur_size--];
                if(heap->cur_size==0)
                    return 0;
                while(i*2<=heap->cur_size){
                   int j=2*i;
                   if(heap->flag){
                      if(j<heap->cur_size){
                          if((long)(heap->arr[j])<(long)(heap->arr[j+1]))
                              j++;
                      }
                      if((long)(tail_value)<(long)(heap->arr[j])){
                          heap->arr[i]=heap->arr[j];
                          i=j;
                      }else
                          break;

                   }else{
                      if(j<heap->cur_size){
                          if((long)(heap->arr[j])>(long)(heap->arr[j+1]))
                              j++;
                      }
                      if((long)(tail_value)>(long)(heap->arr[j])){
                          heap->arr[i]=heap->arr[j];
                          i=j;
                      }else
                          break;
                            
                   }
                }
                heap->arr[i]=tail_value;
                break;
            default:
                return -1;
       }
   }else{
       void * a,*b,*c;
       *(unsigned long*)rt_value=heap->arr[1];
       tail_value=heap->arr[heap->cur_size--];
       if(heap->cur_size==0)
           return 0;
       a=(*(heap->func))(tail_value);
       while(i*2<=heap->cur_size){
           int j;
           j=2*i;
           b=(*(heap->func))(heap->arr[j]);
           if(heap->flag){
               if(j<heap->cur_size){
                   c=(*(heap->func))(heap->arr[j+1]);
                   if(_judge(heap->type,b,c)<0){
                       j++;
                       b=c;     
                   }
               }
               if(_judge(heap->type,a,b)<0){
                   heap->arr[i]=heap->arr[j];
                   i=j;
               }else
                   break;

           }else{
               if(j<heap->cur_size){
                   c=(*(heap->func))(heap->arr[j+1]);
                   if(_judge(heap->type,b,c)>0){
                       j++;
                       b=c;     
                   }
               }
               if(_judge(heap->type,a,b)>0){
                   heap->arr[i]=heap->arr[j];
                   i=j;
               }else
                   break;
          }
     }
     heap->arr[i]=tail_value;
           
   }
   return 0;
}

void
release_heap(struct Heap * heap){
   free(heap->arr);
   free(heap);
}

