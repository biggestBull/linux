#include"./B+Tree.h"

#define B43TREE_HEAD_SIZE 30 

static off_t
_new(int fd,int size,void * ptr){
    off_t rt;

    rt=lseek(fd,0,SEEK_END);
    if(write(fd,ptr,size)!=size)
        return -1;
    return rt;
}

static int
_update(int fd,int size,void * ptr,off_t offset){
   
    lseek(fd,offset,SEEK_SET);
    if(write(fd,ptr,size)!=size)
        return -1;
    return 0;
}

static int
_readTo(int fd,int size,void * ptr,off_t offset){
   
    lseek(fd,offset,SEEK_SET);
    if(read(fd,ptr,size)!=size)
        return -1;
    return  0;
}

static void
_insertKeySort(struct B43Tree_Node * bn,int key,off_t left_son,off_t right_son){
    int i;

    i=bn->cur_key_num-1;
    while(i>=0 && key<bn->sons[i].key){
        bn->sons[i+1]=bn->sons[i];
        i--;
    }
    bn->sons[++i].key=key;
    if(bn->isleave){
       bn->sons[i].ptr=right_son;
    }else{
       bn->sons[i].ptr=left_son;
       if(i<bn->cur_key_num)
           bn->sons[i+1].ptr=right_son;
       else
           bn->other_son=right_son;
    }
    bn->cur_key_num+=1;
}

static int
_hasKey(struct B43Tree_Node * bn,int key){                    //给叶子节点用的
   int i=0;
  
   while(i<bn->cur_key_num){
       if(bn->sons[i].key==key)
           break;
       i++;
   }
   return i<bn->cur_key_num?i:-1; 
}


static int
_split(struct B43Tree * BT,struct B43Tree_Node * bn,off_t l_offset,int mid){
    struct B43Tree_Node parent,right;
    int i,max;
    off_t r_offset;
   
    max=bn->isleave?B43_M+1:B43_M;
    r_offset=lseek(BT->fd,0,SEEK_END);
    
    right.cur_key_num=0;
    bn->cur_key_num=mid;

    for(i=mid+1;i<max;i++){
        right.sons[right.cur_key_num++]=bn->sons[i];
        if(!bn->isleave){
           if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->sons[i].ptr))                        //临时用用
               return -1;
           parent.parent=r_offset;
           if(_update(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->sons[i].ptr))
               return -1;
        }
    }
    if(!bn->isleave){
        if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->other_son))                        //临时用用
           return -1;
        parent.parent=r_offset;
        if(_update(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->other_son))
           return -1;
    }
    right.other_son=bn->other_son;
    bn->other_son=bn->sons[mid].ptr;
    right.isleave=bn->isleave;
    if(bn->isleave){
        _insertKeySort(&right,bn->sons[mid].key,0,bn->sons[mid].ptr);
    }              

    if(!bn->parent){
        memset(&parent,0,sizeof(struct B43Tree_Node));
        _insertKeySort(&parent,bn->sons[mid].key,l_offset,r_offset);
        BT->Root=parent;
        BT->offset=lseek(BT->fd,sizeof(struct B43Tree_Node),SEEK_END);
        bn->parent=BT->offset;
        
        if(_update(BT->fd,sizeof(off_t),&BT->offset,sizeof(int)+2))
            return -1;
    }else{
        if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->parent))
            return -1;
        _insertKeySort(&parent,bn->sons[mid].key,l_offset,r_offset);
        if(!parent.parent)
            BT->Root=parent;

    }
    if(_update(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->parent))
        return -1;
    
    right.parent=bn->parent;
    if(_update(BT->fd,sizeof(struct B43Tree_Node),&right,r_offset))
        return -1;
    if(_update(BT->fd,sizeof(struct B43Tree_Node),bn,l_offset))
        return -1;
   
    if(parent.cur_key_num>=B43_M)
        return _split(BT,&parent,bn->parent,B43_M/2);
 
    return 0;
}

static int
_findLRBrother(struct B43Tree_Node * bn,int key,off_t * l,off_t * r){
    int i;
   
    i=bn->cur_key_num-1;
    while(i>=0 && key < bn->sons[i].key) i--;
    *l=i>=0?bn->sons[i].ptr:0;
    i++;
    *r=i+1<bn->cur_key_num?bn->sons[i+1].ptr:(i+1==bn->cur_key_num)?bn->other_son:0;
    return i;
}

static int
_merge(struct B43Tree * BT,struct B43Tree_Node * bn,off_t offset){
    off_t l_off,r_off;    
    struct B43Tree_Node parent,brother_l,brother_r,temp;
    int p_k,i;

    if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->parent))
        return -1;
    p_k=_findLRBrother(&parent,bn->sons[0].key,&l_off,&r_off);
    if(l_off && _readTo(BT->fd,sizeof(struct B43Tree_Node),&brother_l,l_off))
        return -2;
    if(r_off && _readTo(BT->fd,sizeof(struct B43Tree_Node),&brother_r,r_off))
        return -3;
   
    if(l_off){
        if((brother_l.isleave && brother_l.cur_key_num>(B43_M+1)/2) || (!brother_l.isleave && brother_l.cur_key_num>B43_M/2)){
           i=brother_l.cur_key_num-1;

           if(!bn->isleave){
               _insertKeySort(bn,parent.sons[p_k-1].key,brother_l.other_son,bn->sons[0].ptr);
           }
           else{
               _insertKeySort(bn,brother_l.sons[i].key,0,brother_l.sons[i].ptr);
           }

           if(!bn->isleave){
               if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&temp,brother_l.other_son))
                    return -4;
               temp.parent=offset;
               if(_update(BT->fd,sizeof(struct B43Tree_Node),&temp,brother_l.other_son))
                   return -5;
           }
           parent.sons[p_k-1].key=brother_l.sons[i].key;
           brother_l.other_son=brother_l.sons[i].ptr;
           brother_l.cur_key_num--;

           if(!parent.parent)
               BT->Root=parent;
          
           if(_update(BT->fd,sizeof(struct B43Tree_Node),bn,offset))
               return -6;
           if(_update(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->parent))
               return -7;
           if(_update(BT->fd,sizeof(struct B43Tree_Node),&brother_l,l_off))
               return -8;

           return 0;
        }
    }

    if(r_off){
        if((brother_r.isleave && brother_r.cur_key_num>(B43_M+1)/2) || (!brother_r.isleave && brother_r.cur_key_num>B43_M/2)){
           if(!bn->isleave){
               _insertKeySort(bn,parent.sons[p_k].key,bn->other_son,brother_r.sons[0].ptr);
               parent.sons[p_k].key=brother_r.sons[0].key;
           }
           else{
               _insertKeySort(bn,brother_r.sons[0].key,0,brother_r.sons[0].ptr);
               parent.sons[p_k].key=brother_r.sons[1].key;
           }
           
           if(!bn->isleave){
               if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&temp,brother_r.sons[0].ptr))
                    return -9;
               temp.parent=offset;
               if(_update(BT->fd,sizeof(struct B43Tree_Node),&temp,brother_r.sons[0].ptr))
                   return -10;
           }
          
           brother_r.cur_key_num--;
           for(i=0;i<brother_r.cur_key_num;i++){
               brother_r.sons[i]=brother_r.sons[i+1];     
           }

           if(!parent.parent)
               BT->Root=parent;

           if(_update(BT->fd,sizeof(struct B43Tree_Node),bn,offset))
               return -13;
           if(_update(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->parent))
               return -14;
           if(_update(BT->fd,sizeof(struct B43Tree_Node),&brother_r,r_off))
               return -15;
           return 0;
                    
        }
    }
    
    if(l_off){
      if(!bn->isleave){
          brother_l.sons[brother_l.cur_key_num].key=parent.sons[p_k-1].key; 
          brother_l.sons[brother_l.cur_key_num].ptr=brother_l.other_son; 
      }else{
          brother_l.cur_key_num--;   
      }
      
      for(i=0;i<bn->cur_key_num;i++){
           brother_l.sons[++(brother_l.cur_key_num)]=bn->sons[i];     
           if(!bn->isleave){
               if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&temp,bn->sons[i].ptr))
                   return -16;
               temp.parent=l_off;
               if(_update(BT->fd,sizeof(struct B43Tree_Node),&temp,bn->sons[i].ptr))
                   return -17;
           }
       }
       if(!bn->isleave){
           brother_l.other_son=bn->other_son;
           if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&temp,bn->other_son))
               return -18;
           temp.parent=l_off;
           if(_update(BT->fd,sizeof(struct B43Tree_Node),&temp,bn->other_son))
               return -19;
       }
       brother_l.cur_key_num++;
       
       if(p_k<parent.cur_key_num){
           parent.sons[p_k].ptr=l_off;     
       }else{
           parent.other_son=l_off;     
       }

       for(i=p_k;i<parent.cur_key_num;i++){
           parent.sons[i-1]=parent.sons[i];     
       }
       parent.cur_key_num--;
      
       if(parent.cur_key_num==0 && parent.parent==0){
          BT->offset=l_off;
          brother_l.parent=bn->parent=0;
          BT->Root=brother_l;
          if(_update(BT->fd,sizeof(off_t),&BT->offset,sizeof(int)+2))
              return -22;
          if(_update(BT->fd,sizeof(struct B43Tree_Node),&BT->Root,BT->offset))
              return -23;
          return 0;
       }
      
       if(_update(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->parent))
           return -20;
       if(_update(BT->fd,sizeof(struct B43Tree_Node),&brother_l,l_off))
           return -21;
       
       if(parent.cur_key_num<B43_M/2){
           return _merge(BT,&parent,bn->parent);     
       }else if(parent.parent==0){
          BT->Root=parent;      
       }

       return parent.cur_key_num>=B43_M/2?1:0;
    }else if(r_off){
       if(!bn->isleave){
           bn->sons[bn->cur_key_num].key=parent.sons[p_k].key;
           bn->sons[bn->cur_key_num].ptr=bn->other_son;
       }else{
           bn->cur_key_num--;     
       }
       
       for(i=0;i<brother_r.cur_key_num;i++){
           bn->sons[++(bn->cur_key_num)]=brother_r.sons[i];     
           if(!bn->isleave){
               if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&temp,brother_r.sons[i].ptr))
                   return -24;
               temp.parent=offset;
               if(_update(BT->fd,sizeof(struct B43Tree_Node),&temp,brother_r.sons[i].ptr))
                   return -25;
           }
       }
       if(!bn->isleave){
           bn->other_son=brother_r.other_son;
           if(_readTo(BT->fd,sizeof(struct B43Tree_Node),&temp,brother_r.other_son))
               return -26;
           temp.parent=offset;
           if(_update(BT->fd,sizeof(struct B43Tree_Node),&temp,brother_r.other_son))
               return -27;
       }
       bn->cur_key_num++;
       if(parent.cur_key_num>p_k+1){     
           parent.sons[p_k+1].ptr=offset;
           for(i=p_k+1;i<parent.cur_key_num;i++){
               parent.sons[i-1]=parent.sons[i];     
           }
       }else{
           parent.other_son=offset;
       }
       parent.cur_key_num--;
      
       if(parent.cur_key_num==0 && parent.parent==0){
          BT->offset=offset;
          bn->parent=0;
          BT->Root=*bn;
          if(_update(BT->fd,sizeof(off_t),&BT->offset,sizeof(int)+2))
              return -29;
          if(_update(BT->fd,sizeof(struct B43Tree_Node),&BT->Root,BT->offset))
              return -30;
          return 0;
       }
      
       if(_update(BT->fd,sizeof(struct B43Tree_Node),&parent,bn->parent))
           return -28;
       if(_update(BT->fd,sizeof(struct B43Tree_Node),bn,offset))
           return -29;
       
       if(parent.cur_key_num<B43_M/2){
          return  _merge(BT,&parent,bn->parent);     
       }
       else if(!parent.parent)
           BT->Root=parent;
       
       return parent.cur_key_num>=B43_M/2?0:1;
    } 
    return -31;

}

static int
_search(int fd,struct B43Tree_Node * son,off_t * ptr,int key){
   int i;

   while(!son->isleave){
      for(i=0;i<son->cur_key_num;i++){
          if(son->sons[i].key>key)
             break;
      }
      *ptr=i>=son->cur_key_num?son->other_son:son->sons[i].ptr;
      if(_readTo(fd,sizeof(struct B43Tree_Node),son,*ptr))
          return -1;
   }
   return 0;
}

struct B43Tree *
createB43Tree(char * filename,int data_size){
   struct B43Tree * Root;
   char string[B43TREE_HEAD_SIZE];
   int fd;

   if((fd=open(filename,O_RDWR | O_EXCL | O_CREAT,S_IRWXU,S_IRGRP))==-1)
       return NULL;
   if((Root=malloc(sizeof(struct B43Tree)))==NULL)
       return NULL;
   Root->fd=fd;
   Root->data_size=data_size;
   Root->offset=B43TREE_HEAD_SIZE;
   Root->Root.cur_key_num=0;
   Root->Root.isleave=1;
   Root->Root.other_son=0;
   Root->Root.parent=0;
   
   string[0]='B';
   string[1]='+';
   *((int *)(string+2))=data_size;
   *((off_t *)(string+sizeof(int)+2))=B43TREE_HEAD_SIZE;

   if(write(fd,string,B43TREE_HEAD_SIZE)!=B43TREE_HEAD_SIZE){
       free(Root);
       unlink(filename);
       close(fd);
       return NULL;
   }
   if(write(fd,&Root->Root,sizeof(Root->Root))!=sizeof(Root->Root)){
       free(Root);
       unlink(filename);
       close(fd);
       return NULL;
   }
   return Root;
}


struct B43Tree *
openB43Tree(char * filename){
   struct B43Tree * Root;
   int fd;
   char string[B43TREE_HEAD_SIZE];

   if((fd=open(filename,O_RDWR))==-1)
       return NULL;
   if(read(fd,string,B43TREE_HEAD_SIZE)!=B43TREE_HEAD_SIZE){
       close(fd);
       return NULL;
   }
   if(string[0]=='B' && string[1]=='+'){
       if((Root=malloc(sizeof(struct B43Tree)))==NULL){
           close(fd);
           return NULL;
       }
       Root->fd=fd;
       Root->data_size=*((int *)(string+2));
       Root->offset=*((off_t *)(string+sizeof(int)+2));
       lseek(fd,Root->offset,SEEK_SET);
       if(read(fd,&Root->Root,sizeof(Root->Root))!=sizeof(Root->Root)){
           close(fd);
           free(Root);
           return NULL;
       }
       return Root;
   }
   close(fd);
   return NULL;
}


int
B43Tinsert(struct B43Tree * BT,int key,void * data){
   struct B43Tree_Node son;
   int i;
   off_t ptr,new_off;

   son=BT->Root;
   ptr=BT->offset;
   i=_search(BT->fd,&son,&ptr,key);
   if(i==-1)
       return -1;
   i=_hasKey(&son,key);
   if(i==-1){
       if((new_off=_new(BT->fd,BT->data_size,data))==-1)
           return -1;
       _insertKeySort(&son,key,0,new_off);
       if(son.cur_key_num>B43_M){
           i=_split(BT,&son,ptr,(B43_M+1)/2);
           if(i==-1)
               return -1;
       }else{
           if(son.parent==0)
               BT->Root=son;
           if(_update(BT->fd,sizeof(struct B43Tree_Node),&son,ptr))
               return -1;
       }
   }else{
       if(_update(BT->fd,BT->data_size,data,son.sons[i].ptr))
           return -1;
   }
   return 0;
}

int
B43Tsearch(struct B43Tree * BT,int key,void * data_p){
   struct B43Tree_Node son;
   int i;
   off_t ptr;

   son=BT->Root;
   ptr=BT->offset;
   if(_search(BT->fd,&son,&ptr,key)==-1)
       return -1;

   i=_hasKey(&son,key);
   if(i==-1){
       return 1;      
   }
   if(_readTo(BT->fd,BT->data_size,data_p,son.sons[i].ptr))
       return -1;
   return 0;
}

int
B43Tdelete(struct B43Tree * BT,int key){
   struct B43Tree_Node son;
   int i;
   off_t ptr;

   son=BT->Root;
   ptr=BT->offset;
   if(_search(BT->fd,&son,&ptr,key)==-1)
       return -1;
   i=_hasKey(&son,key);
   if(i==-1){
       return 1;     
   }
   for(i=i+1;i<son.cur_key_num;i++){
       son.sons[i-1]=son.sons[i];     
   }
   son.cur_key_num--;
   if(_update(BT->fd,sizeof(struct B43Tree_Node),&son,ptr))
       return -1;
   if(son.parent){
       
      if(son.cur_key_num<(B43_M+1)/2){
          i=1;
          i=_merge(BT,&son,ptr); 
          if(i<0)
             return i;
      }
      
   }else{
      BT->Root=son;      
   }
   return 0;          
}







