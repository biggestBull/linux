#include"./Huffman.h"

static void
_setBit(char * a,char sit){
   *a|=1<<sit;        
}

static void
_clearBit(char * a,char sit){
   *a&=~(1<<sit);        
}

static int
_getBit(char a,char sit){
   return (a&(1<<sit))==0?0:1;        
}

static int
_getCharRate(int fd_in,long * char_rate){
   char  inbuffer[HFM_BUFFER_SIZE];
   int i,j;

   while((i=read(fd_in,inbuffer,HFM_BUFFER_SIZE))>0){
       for(j=0;j<i;j++){
           char_rate[(unsigned char)(inbuffer[j])]++;     
       }
   }
   lseek(fd_in,0,SEEK_SET);
   return i;
}

void *
_getCount(void * addr){
   return ((struct Huffman_Node *)(addr))->count;        
}

static struct Huffman_Node *
_createHFMTree(long * char_rate){
   struct Heap * priority_queue;
   struct Huffman_Node * Node=NULL,*Node1,*Node2;
   int i;

   if((priority_queue=create_Heap(LONG,0,_getCount))==NULL)
       return NULL;

   for(i=0;i<256;i++){
      if(char_rate[i]==0)
          continue;
      if((Node=malloc(sizeof(struct Huffman_Node)))==NULL)
          return NULL;
      Node->c=i;
      Node->left=Node->right=NULL;
      Node->count=char_rate[i];
      heap_insert(priority_queue,Node); 
   }
   while(heap_pop(priority_queue,&Node1)==0 && heap_pop(priority_queue,&Node2)==0){
      if((Node=malloc(sizeof(struct Huffman_Node)))==NULL)
          return NULL;
      Node->left=Node1;
      Node->right=Node2;
      Node->count=Node1->count+Node2->count;
      heap_insert(priority_queue,Node); 
   }
   release_heap(priority_queue);
   return Node;
}

static void
_getHFMCode(unsigned long * char_code,struct Huffman_Node * Root,char * s,int s_index){
   if(Root->left==Root->right){
       char * temp;
       temp=malloc(strlen(s)+1);
       strncpy(temp,s,strlen(s));
       temp[strlen(s)]='\0';
       char_code[(unsigned char)(Root->c)]=temp;
   }else{
       s[s_index]='1';
       s[s_index+1]='\0';
       _getHFMCode(char_code,Root->left,s,s_index+1);
       s[s_index]='0';
       s[s_index+1]='\0';
       _getHFMCode(char_code,Root->right,s,s_index+1);
   }
}

static unsigned long
_constructHeader(int fd_out,long * char_rate,unsigned long * char_code){
   int i,j;
   unsigned long sum=0;
   long file_size=0;
   char temp[100];

   for(i=0;i<256;i++){
       if(char_rate[i]==0)
           continue;
       snprintf(temp,100,"[%d:%s]\n",i,char_code[i]);
       j=strlen(temp);
       if(write(fd_out,temp,j)!=j)
           return -1;
       sum+=char_rate[i]*strlen(char_code[i]);
       file_size+=char_rate[i];
   }
   snprintf(temp,100,"[<%ld]",sum);
   j=strlen(temp);
   if(write(fd_out,temp,j)!=j)
       return -1;
   return file_size;
}

static int
_printPercent(int lastlen,unsigned long cur,unsigned long all,char * arr_buf,int buf_len){
    int i=0;
    int a;
    a=(int)((double)(cur)/(double)(all)*100.0);
    while(i<4)
        printf("\b");
    snprintf(arr_buf,buf_len,"%0d%%",a);
    printf("cur:%s\n",arr_buf);
    //fflush(stdout);                        //尼玛，，，又记混了。
    return strlen(arr_buf);
}

static int
_constructBody(int fd_out,int fd_in,unsigned long * char_code,unsigned long file_size,int flag){
   char inbuffer[HFM_BUFFER_SIZE],outbuffer[HFM_BUFFER_SIZE];
   int outbuf_i=0,i;
   unsigned long count=0;
   int lastlen=0;
   char temp[100];
   while((i=read(fd_in,inbuffer,HFM_BUFFER_SIZE))>0){
       int j;
       for(j=0;j<i;j++){
           int k;
           char *s;
           s=char_code[(unsigned char)(inbuffer[j])];
           for(k=0;k<strlen(s);k++){
              if(s[k]=='1'){
                  _setBit(outbuffer+outbuf_i/8,outbuf_i%8);   
              }else{
                  _clearBit(outbuffer+outbuf_i/8,outbuf_i%8);   
              }
              outbuf_i++;
              if(outbuf_i==HFM_BUFFER_SIZE*8){
                 if(write(fd_out,outbuffer,HFM_BUFFER_SIZE)!=HFM_BUFFER_SIZE)
                     return -1;
                 count+=(outbuf_i+7)/8;
                 outbuf_i=0;
               /*  if(flag && flush_count==8){
                     flush_count=0;
                     lastlen=_printPercent(lastlen,count,file_size,temp,100);     
                 }
                 */
              }
           }
       }
   }
   if(write(fd_out,outbuffer,(outbuf_i+7)/8)!=(outbuf_i+7)/8)
       return -1;
   if(flag)
       printf("100%\n");
   return 0;
}

static int
_writeToFile(int fd_in,int fd_out,long * char_rate,unsigned long * char_code,int flag){
   unsigned long file_size;
   if((file_size=_constructHeader(fd_out,char_rate,char_code))==-1)
       return -1;
   if(_constructBody(fd_out,fd_in,char_code,file_size,flag)==-1)
       return -1;
   return 0;
}

static int
_insertToTree(struct Huffman_Node * Root,char * s,char c){
   struct Huffman_Node * temp=Root;

   while(*s){
       if(*s=='1'){
           if(!temp->left){
               if((Root=malloc(sizeof(struct Huffman_Node)))==NULL)
                   return -1;
               Root->left=Root->right=NULL;
               temp->left=Root;
           }
           temp=temp->left;
       }else{
           if(!temp->right){
               if((Root=malloc(sizeof(struct Huffman_Node)))==NULL)
                   return -1;
               Root->left=Root->right=NULL;
               temp->right=Root;
           }
           temp=temp->right;
       }
       s++;
   }
   temp->c=c;

   return 0;
}

static struct Huffman_Node * 
_restoreHFMTree(unsigned long * char_code){
   struct Huffman_Node * Root;
   int i;
   if((Root=malloc(sizeof(struct Huffman_Node)))==NULL)
       return NULL;
   Root->left=Root->right=NULL;
   for(i=0;i<256;i++){
       if(char_code[i]==0)   
           continue;
       if(_insertToTree(Root,char_code[i],i)==-1)
           return NULL;
   }
   return Root;
}

static struct Huffman_Node *
_handleHeader(char * buf,int * flag,unsigned long * char_code){
   char * s;
   int i=1;
   unsigned char c=0;
   unsigned long sum=0;
   struct Huffman_Node * Root=NULL;
   
   while(buf[i]<=57 && buf[i]>=48){
       c=c*10+buf[i++]-48;     
   }

   if(buf[i]==':'){
       s=malloc(strlen(buf+i+1)+1);
       strncpy(s,buf+1+i,strlen(buf+i+1));
       s[strlen(buf+i+1)]='\0';
       char_code[c]=s;
   }else{
       *flag=1;
       i++;
       while(buf[i]){
           sum=sum*10+buf[i]-48;
           i++;
       }
       char_code[256]=sum;
       Root=_restoreHFMTree(char_code);
   }
   return Root;
}

static int
_decompressHandler(int fd_in,int fd_out,int flag){
   int i,outbuf_i=0,flag_headend=0,flag_incode=0,lastlen=0;
   unsigned long char_code[257];
   char inbuffer[HFM_BUFFER_SIZE],outbuffer[HFM_BUFFER_SIZE],tpbf[100],tpbf_i=0;
   struct Huffman_Node * Root=NULL,*temp=NULL;
   unsigned long sum=0;

   memset(char_code,0,sizeof(unsigned long)*257);
   while((i=read(fd_in,inbuffer,HFM_BUFFER_SIZE))>0){
       int j;
       for(j=0;j<i;j++){
           if(!flag_headend){
               if(inbuffer[j]=='[' || flag_incode){
                   flag_incode=1;
                   if(inbuffer[j]==']' && tpbf_i!=1){
                       temp=Root=_handleHeader(tpbf,&flag_headend,char_code); 
                       tpbf_i=0;
                       flag_incode=0;
                       continue;
                   }
                   tpbf[tpbf_i]=inbuffer[j];
                   tpbf[++tpbf_i]='\0';
               }
           }else{
               int k;
               char c;
               c=inbuffer[j];
               if(Root==NULL)
                   return -1;
               for(k=0;k<8;k++){
                  if(sum>=char_code[256]){
                      if(write(fd_out,outbuffer,outbuf_i)!=outbuf_i)
                          return -1;
                      if(flag)
                          printf("100%\n");
                      return 0;
                  }
                  if(outbuf_i>=HFM_BUFFER_SIZE){
                      if(write(fd_out,outbuffer,HFM_BUFFER_SIZE)!=HFM_BUFFER_SIZE)
                          return -1;
                     // if(flag)
                       //   lastlen=_printPercent(lastlen,sum,char_code[256],tpbf,100);
                      outbuf_i=0;
                  }
                  if(_getBit(c,k)==1){
                      temp=temp->left;   
                  }else{
                      temp=temp->right;   
                  }
                  if(temp->left==temp->right){
                     outbuffer[outbuf_i++]=temp->c;
                     temp=Root;
                  }
                  sum++;
               }
           }
       }
       
   }
   return 0;
}

int
Huffman_compress(const char * filepath,const char * outfilepath,int flag){
   int fd_in,fd_out;
   long char_rate[256],char_code[256];
   struct Huffman_Node * Root;
   char temp[100];

   memset(char_rate,0,sizeof(long)*256);
   memset(char_code,0,sizeof(long)*256);
   if((fd_in=open(filepath,O_RDONLY))==-1)
       return -1;
   if((fd_out=open(outfilepath,O_WRONLY| O_CREAT |O_TRUNC,S_IRWXU | S_IRGRP | S_IROTH))==-1)
       return -1;
   if(_getCharRate(fd_in,char_rate)==-1)
       return -1;
   if((Root=_createHFMTree(char_rate))==NULL)
       return -1;
   if(Root->left==Root->right)
       return 1;                                          //单字符，无法压缩
   _getHFMCode(char_code,Root,temp,0);
   if(_writeToFile(fd_in,fd_out,char_rate,char_code,flag)==-1)
       return -1;
}

int
Huffman_decompress(const char * filepath,const char * outfilepath,int flag){
   int fd_in,fd_out;        
   
   if((fd_in=open(filepath,O_RDONLY))==-1)
       return -1;
   if((fd_out=open(outfilepath,O_WRONLY| O_CREAT |O_TRUNC,S_IRWXU | S_IRGRP | S_IROTH))==-1)
       return -1;
   return _decompressHandler(fd_in,fd_out,flag);
}





























