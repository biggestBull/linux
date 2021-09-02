#include"./sort.h"
#include"../../data_structure/Queue/queue.h"
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#define CHAR_BUFFER_SIZE 1024
#define OTHER_MEM 2048
#define MIN_MEM_SIZE 4096 
#define _MAX_INT (unsigned int)(-1)/2
#define _MIN_INT 1<<sizeof(int)*8-1

typedef struct {
   int fd;
   int sector_size;
   int cur_size;
   int cur_pos;
   int remain_size;
   int buffer_size;
   int * buffer;
}file_info;

struct Out_IN{
   file_info * in;
   file_info * out;
};

static char char_buf[CHAR_BUFFER_SIZE];

static int
_read_data_from_fd(int fd,file_info * in,int num,struct Queue * queue_in,int flag,char c){
   int sum=0,temp,i,count,all=0,infile_count=0;
   
   for(i=1;i<num;i++){
       queue_push(queue_in,in+i);     
   }
   lseek(fd,0,SEEK_SET);
   while((count=read(fd,char_buf,CHAR_BUFFER_SIZE))>0){
       for(i=0;i<count;i++){
           if(in->remain_size>=in->buffer_size){
               heap_sort(in->buffer,in->buffer_size,flag);     
               if(write(in->fd,in->buffer,in->buffer_size*sizeof(int))!=in->buffer_size*sizeof(int))
                   return -1;
               in->remain_size=0;
               in->cur_pos=1;
               queue_push(queue_in,in);
               queue_pop(queue_in,&in);
           }
           if(char_buf[i]==c){
               in->buffer[in->remain_size++]=sum;
               sum=0;
               continue;
           }
           sum=sum*10+char_buf[i]-48;
       }
   }
   if(in->remain_size!=0){
       heap_sort(in->buffer,in->remain_size,flag);     
       if(write(in->fd,in->buffer,in->remain_size*sizeof(int))!=in->remain_size*sizeof(int))
           return -1;
       in->cur_pos=1;
       in->remain_size=0;
   }
   do{
      if(in->cur_pos){
           lseek(in->fd,0,SEEK_SET);
           in->sector_size=in->buffer_size; 
      }
   }while(queue_pop(queue_in,&in)!=-1);
   return 0;
}

static struct Out_IN *
_init(int mem_size,int num,int fd,struct Queue * queue_in,int flag,char c){
   int i,buf_size;
   FILE * temp;
   struct Out_IN * out_in;

   if((out_in=malloc(sizeof(struct Out_IN)))==NULL)
       return NULL;
   if((out_in->in=malloc(sizeof(file_info)*num))==NULL){
       free(out_in);    
       return NULL;
   }
   if((out_in->out=malloc(sizeof(file_info)*num))==NULL){
       free(out_in->in);
       free(out_in);
       return NULL;
   }
   buf_size=mem_size/num;
   buf_size/=sizeof(int);
   for(i=0;i<num;i++){
      if((out_in->in[i].buffer=malloc(sizeof(int)*buf_size))==NULL)
          return NULL;
      out_in->in[i].buffer_size=buf_size;
      if((temp=tmpfile())==NULL)
          return NULL;
      if((out_in->in[i].fd=fileno(temp))==-1)
          return NULL;
      out_in->in[i].sector_size=0;
      out_in->in[i].remain_size=0;
      out_in->in[i].cur_pos=0;

   }
   for(i=0;i<num;i++){
      if((out_in->out[i].buffer=malloc(sizeof(int)*buf_size))==NULL)
          return NULL;
      out_in->out[i].buffer_size=buf_size;
      if((temp=tmpfile())==NULL)
          return NULL;
      if((out_in->out[i].fd=fileno(temp))==-1)
          return NULL;
      out_in->out[i].sector_size=0;
      out_in->out[i].remain_size=0;
   }
   if(_read_data_from_fd(fd,out_in->in,num,queue_in,flag,c)==-1)
       return NULL;
   return out_in;
}

static int
_writeToFile(file_info * fi,int fd,int flag,char c){
   char temp[20];
   int count=0,j=0, pos=0,pre_num,i,len;
   lseek(fd,0,SEEK_SET); 
   while((count=read(fi->fd,fi->buffer,fi->buffer_size*sizeof(int)))>0){
       count/=sizeof(int);
       pre_num=fi->buffer[0];
       for(i=0;i<count;i++){
           snprintf(temp,20,"%d%c",fi->buffer[i],c);
           len=strlen(temp);
           if(j+len>=CHAR_BUFFER_SIZE){
               if(write(fd,char_buf,j)!=j)
                   return -1;
               j=0;
           }
           strncpy(char_buf+j,temp,len);
           j+=len;
           if(flag){
              if(fi->buffer[i]>pre_num){
                  break;   
              }
           }else{
              if(fi->buffer[i]<pre_num){
                  break;   
              }
                    
           }
           pos++;
       }
   }
   if(j){
       if(write(fd,char_buf,j)!=j)
           return -1;
   }
   return i>=count?0:pos;
}

static void
_adjust(int * successTree,int * leaves,int flag,int k,int i){
  int m,n;

  if(2*i<k){
     m=successTree[2*i];    
  }else{
     m=2*i-k;    
  }

  if(2*i+1<k){
     n=successTree[2*i+1];
  }else{
     n=2*i-k+1;    
  }
  
  if(flag){
     successTree[i]=leaves[m]>leaves[n]?m:n;    
  }else{
     successTree[i]=leaves[m]<leaves[n]?m:n;    
  }

}

static void
_adjustToRoot(int * successTree,int * leaves,int flag,int k,int i){
  
   i=(i+k)/2; 
   while(i>0){
       _adjust(successTree,leaves,flag,k,i);
       i/=2;
   }
}

static void
_initSuccessTree(int * successTree,int * leaves,int flag,int k){
   int i;

   for(i=k-1;i>0;i--){
       _adjust(successTree,leaves,flag,k,i);
   }
}

static int
_start(struct Out_IN * out_in,int num,struct Queue * queue_out,int flag){
   int i,temp,in_count1=0,in_count2=0,out_count=0,rd,temp_sector_size=0,remain_count,extreme,*successTree,*leaves;
   file_info * in,*out;

   extreme=flag?_MIN_INT:_MAX_INT;
   if((successTree=malloc(sizeof(int)*(num+1)))==NULL)
       return -1;
   if((leaves=malloc(sizeof(int)*(num+1)))==NULL)
       return -1;
   leaves[num]=extreme;

   while(out_count!=1){
       remain_count=0;
       in_count1=0;
       for(i=0;i<num;i++){
           lseek(out_in->in[i].fd,0,SEEK_SET);
           lseek(out_in->out[i].fd,0,SEEK_SET);
           ftruncate(out_in->out[i].fd,0);
           
           if(out_in->in[i].sector_size!=0){
                    
               if((rd=read(out_in->in[i].fd,out_in->in[i].buffer,out_in->in[i].buffer_size*sizeof(int)))<0){
                   return -2;
               }
               if(rd==0){
                   leaves[i]=extreme;  
                   out_in->in[i].sector_size=0;
                   continue;
               }
               remain_count++;
               out_in->in[i].cur_pos=0;
               out_in->in[i].remain_size=rd/sizeof(int);
               leaves[i]=out_in->in[i].buffer[out_in->in[i].cur_pos];
               out_in->in[i].cur_size=0;
           
           }else{
               leaves[i]=extreme;                    
           }

           if(queue_push(queue_out,out_in->out+i)==-1)
               return -3;
           out_in->out[i].sector_size=0;
       }

       _initSuccessTree(successTree,leaves,flag,num+1);

       if(queue_pop(queue_out,&out)==-1)
           return -4;
       out_count=1;
       in_count2=remain_count;
       while(1){
           int cur_in_index;
           while(in_count2){
               cur_in_index=successTree[1];
               in=out_in->in+cur_in_index;

               temp=leaves[cur_in_index];
               out->buffer[(temp_sector_size++)%out->buffer_size]=temp;
               if(temp_sector_size%out->buffer_size==0 && temp_sector_size>0){
                   if(write(out->fd,out->buffer,out->buffer_size*sizeof(int))!=out->buffer_size*sizeof(int))
                       return -7;
               }
               in->cur_size++;
               in->cur_pos++;
               if(in->cur_pos>=in->remain_size){
                   if((rd=read(in->fd,in->buffer,in->buffer_size*sizeof(int)))<0)
                       return -6;
                   if(rd==0){
                       in->sector_size=0;
                       leaves[cur_in_index]=extreme;
                       _adjustToRoot(successTree,leaves,flag,num+1,cur_in_index);
                       in_count1++;       
                       in_count2--;
                       continue;
                   }
                   in->remain_size=rd/sizeof(int);
                   in->cur_pos=0;
               }
           
               if(in->cur_size>=in->sector_size){
                   in_count2--;     
                   leaves[cur_in_index]=extreme;
               }else{
                   leaves[cur_in_index]=in->buffer[in->cur_pos];
               }
               _adjustToRoot(successTree,leaves,flag,num+1,cur_in_index);
       }
       
       if(temp_sector_size%out->buffer_size){
            if(write(out->fd,out->buffer,(temp_sector_size%out->buffer_size)*sizeof(int))!=(temp_sector_size%out->buffer_size)*sizeof(int))
                 return -7;
       }
       
       if(in_count1==remain_count){
           file_info * temp_fi;
           in_count1=0;
           in_count2=0;
            if(out->sector_size==0)
                out->sector_size=temp_sector_size;
            temp_sector_size=0;

            if(out_count!=1){
                    
                temp_fi=out_in->in;
                out_in->in=out_in->out;
                out_in->out=temp_fi;
                while(queue_pop(queue_out,&out)!=-1);
            }

            break;
       }
       out_count++;

       if(out->sector_size==0)
           out->sector_size=temp_sector_size;
       temp_sector_size=0;

       if(queue_push(queue_out,out)==-1)
           return -11;
       if(queue_pop(queue_out,&out)==-1)
           return -10;  
       in_count2=0;

       for(i=0;i<num;i++){
           if(out_in->in[i].sector_size==0){
               leaves[i]=extreme;
               continue;
            }
            in=out_in->in+i;
            if(in->cur_pos>=in->remain_size ){
               if((rd=read(in->fd,in->buffer,in->buffer_size*sizeof(int)))<0)
                   return -6;
               if(rd==0){
                   in->sector_size=0;
                   leaves[i]=extreme;
                   continue;
               }
               in->remain_size=rd/sizeof(int);
               in->cur_pos=0;
           }
           leaves[i]=in->buffer[in->cur_pos];
           out_in->in[i].cur_size=0;
           in_count2++;
       }
       _initSuccessTree(successTree,leaves,flag,num+1);
    }
    
   }
   free(successTree);
   free(leaves);
   lseek(out_in->out->fd,0,SEEK_SET);

   return 0;
}

static void
_close(struct Out_IN * out_in,int num,struct Queue * queue_out){
   int i;  

   for(i=0;i<num;i++){
       free(out_in->in[i].buffer);
       free(out_in->out[i].buffer);
   }
   free(out_in->in);
   free(out_in->out);
   free(out_in);
   queue_release(queue_out);
}

int
external_sort(int fd,int flag,int memsize,int num,char interval_char){
   struct Out_IN * out_in;
   int i;
   struct Queue  queue_out;

   num=num>32?32:num;
   memsize=memsize-OTHER_MEM;
   if(memsize<MIN_MEM_SIZE || num<2)
       return -1;
   if(create_queue(&queue_out,PTR)==-1)
       return -3;
   if((out_in=_init(memsize,num,fd,&queue_out,flag,interval_char))==NULL)
       return -5;
   if((i=_start(out_in,num,&queue_out,flag))<=-1){
       return -6;
   }
   i=_writeToFile(out_in->out,fd,flag,interval_char);
   _close(out_in,num,&queue_out);
   return i;
}
