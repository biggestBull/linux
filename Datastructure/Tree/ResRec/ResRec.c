#include"./ResRec.h"                 //我突然发现，，，压根就没必要把先序序列号写进去啊，，，不过写都写了，，，罢了罢了。(最终仍是改了，，，强迫症晚期)
#define BUF_SIZE 2048 

static int 
_inorder(general_st * gst,int fd,int seq,int * max,int *buf_i,char * buf,int * byte_sum,char * string){
    int len,stats=0;
    ++*max;
    if(gst->l)
        stats=_inorder(gst->l,fd,seq+1,max,buf_i,buf,byte_sum,string);
    if(stats==-1)
        return -1;
    snprintf(string,20,"%d ",seq);
    len=strlen(string);
    *byte_sum+=len;
    if(*buf_i+len>=BUF_SIZE){
       if(write(fd,buf,*buf_i)!=*buf_i)
           return -1;
       *buf_i=0;
    }
    strncpy(buf+*buf_i,string,len);
    *buf_i+=len;
    if(gst->r)
       stats=_inorder(gst->r,fd,*max+1,max,buf_i,buf,byte_sum,string);
    if(!(gst->l) && !(gst->r))
        return 0;
    return stats?-1:0;
}

static int 
_preorder(general_st * gst,int size,int fd,int *buf_i,int *databuf_i,char * data_buf,char * string){
    int len,stats=0;
    if(*databuf_i+size-2*sizeof(unsigned long)>=BUF_SIZE){
       if(write(fd,data_buf,*databuf_i)!=*databuf_i)
           return -1;
       *databuf_i=0;
    }
    for(len=sizeof(unsigned long)*2;len<size;len++){
       data_buf[(*databuf_i)++]=*((char *)((char *)gst+len));        
    }
    if(gst->l)
        stats=_preorder(gst->l,size,fd,buf_i,databuf_i,data_buf,string);
    if(stats==-1)
        return -1;
    if(gst->r)
        stats=_preorder(gst->r,size,fd,buf_i,databuf_i,data_buf,string);
    if(!gst->l && !gst->r)
        return 0;
    return stats?-1:0;
}



static int
_readSeqFromFile(int * in_arr,int byte_sum,int fd,char * buf){
   int i,index=0,readed,sum=0;
   
   while(byte_sum){
       if((readed=read(fd,buf,byte_sum%(BUF_SIZE+1)))<0)
           return -1;
       for(i=0;i<readed;i++){
           if(buf[i]==' '){
               in_arr[index++]=sum;
               sum=0;
               continue;
           }
           sum=sum*10+buf[i]-48;
       }
       byte_sum-=byte_sum%(BUF_SIZE+1);
   }
   return 0;
}

static general_st *
_preInRescursive(int size,int * in_arr,int s,int e,int * cur_index,int fd,char * buf,int * curbuf_i,int * buf_i){
   int i,buf_size;
   general_st * Root;

   for(i=s;i<=e;i++){
       if(in_arr[i]==*cur_index)
           break;
   }
   if(i>e)
       return NULL;
   if(*buf_i==*curbuf_i){
       buf_size=BUF_SIZE;
       while(buf_size%(size-sizeof(unsigned long)*2))
           buf_size--;
       if((buf_size=read(fd,buf,buf_size))<=0){
           return NULL;
       }
       *buf_i=buf_size/(size-sizeof(unsigned long)*2);
       *curbuf_i=0;
   }
   if((Root=malloc(size))==NULL)
       return NULL;
   ++*cur_index;
   for(buf_size=sizeof(unsigned long)*2;buf_size<size;buf_size++){
      *((char *)((char *)Root+buf_size))=buf[*curbuf_i*(size-sizeof(unsigned long)*2)+buf_size-2*sizeof(unsigned long)];
   }
   *curbuf_i+=1;
   Root->l=i>s?_preInRescursive(size,in_arr,s,i-1,cur_index,fd,buf,curbuf_i,buf_i):NULL;
   Root->r=i<e?_preInRescursive(size,in_arr,i+1,e,cur_index,fd,buf,curbuf_i,buf_i):NULL;
   return Root;
}

int 
restoreBinTree(general_st * gst,int size,char * filename){
   int fd,max=-1,buf_i=0,byte_sum=0,databuf_i=0,len;
   char buf[BUF_SIZE],string[30];
   
   if((fd=open(filename,O_CREAT | O_RDWR | O_EXCL,S_IRUSR| S_IWUSR |S_IRGRP))==-1)
       if(errno==EEXIST)
           return 1;
       else
           return -1;
   ftruncate(fd,0);
   lseek(fd,30,SEEK_SET);
   if(_inorder(gst,fd,0,&max,&buf_i,buf,&byte_sum,string)==-1){
       close(fd);     
       return -1;
   }
   if(buf_i){
       if(write(fd,buf,buf_i)!=buf_i){
           close(fd);     
           return -1;
       }
       buf_i=0;
   }
   if(_preorder(gst,size,fd,&buf_i,&databuf_i,buf,string)==-1){
       close(fd);     
       return -1;
   }
   if(databuf_i){
       if(write(fd,buf,databuf_i)!=databuf_i){
           close(fd);     
           return -1;
       }
   }
   lseek(fd,0,SEEK_SET);
   snprintf(string,15,"%d",byte_sum);
   len=strlen(string);
   string[len++]=';';
   snprintf(string+len,15,"%d",max+1);
   len+=strlen(string+len);
   string[len++]=';';
   if(write(fd,string,len)!=len){
       close(fd);
       return -1;
   }
   return fd;
}

general_st *
recoverBinTree(int size,char * filename){
   int fd,byte_sum=0,max=0,*in_arr,i=0,buf_i=0,curbuf_i=0;
   char buf[BUF_SIZE];
   general_st * Root;

   if((fd=open(filename,O_RDONLY))==-1)
       return NULL;
   lseek(fd,0,SEEK_SET);
   if(read(fd,buf,30)!=30){
       close(fd);
       return NULL;
   }
   while(buf[i]!=';'){
       byte_sum=byte_sum*10+buf[i++]-48;     
   }
   ++i;
   while(buf[i]!=';'){
       max=max*10+buf[i++]-48;     
   }
   if((in_arr=malloc(sizeof(int)*max))==NULL){
       close(fd);
       return NULL;
   }
   if(_readSeqFromFile(in_arr,byte_sum,fd,buf)!=0){
       close(fd);
       free(in_arr);
       return NULL;
   }
   i=0;
   Root=_preInRescursive(size,in_arr,0,max-1,&i,fd,buf,&curbuf_i,&buf_i);
   close(fd);
   free(in_arr);
   
   return Root;
}

















