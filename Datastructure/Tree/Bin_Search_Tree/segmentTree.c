#include"./segmentTree.h"

#define LeftC(i) ((i<<1)+1)
#define RightC(i) ((i<<1)+2)
#define Parent(i) ((i-1)>>1)
#define ST_MAX_INT ((unsigned int)(-1)/2)
#define ST_MIN_INT (1<<sizeof(int)*8-1)

static void
_pushUp_res(seg_tree_res_st * str,int k){
   if(str->flag)
      str->T[k]=str->T[LeftC(k)]>str->T[RightC(k)]?str->T[LeftC(k)]:str->T[RightC(k)];
   else
      str->T[k]=str->T[LeftC(k)]<str->T[RightC(k)]?str->T[LeftC(k)]:str->T[RightC(k)];
}

static void
_pushDown_res(seg_tree_res_st * str,int k){
   if(str->Lazy[k]){
       str->Lazy[LeftC(k)]+=str->Lazy[k];
       str->Lazy[RightC(k)]+=str->Lazy[k];
       str->T[LeftC(k)]+=str->Lazy[k];                  //记住是加Lazy[k]，不要搞作了
       str->T[RightC(k)]+=str->Lazy[k];
       str->Lazy[k]=0;                                   //lazy是用来加的，所以赋值为0是不会有影响的
   }
}

static int
_initST_res(seg_tree_res_st * str,int * nums,int k,int l,int r){
   int m;
   if(l==r){
      str->T[k]=nums[l];
   }else{        
      m=l+((r-l)>>1);
      _initST_res(str,nums,LeftC(k),l,m);
      _initST_res(str,nums,RightC(k),m+1,r);
      _pushUp_res(str,k);
      str->Lazy[k]=0;
   }
   return str->T[k];
}

static int
_queryST_res(seg_tree_res_st * str,int k,int L,int R,int l,int r){
   int lv,rv,m;
   
   if(((l-L)|(R-r))>=0){                       //见tlpi上笔记
      return str->T[k];      
   }
   m=l+((r-l)>>1);
   _pushDown_res(str,k);
   rv=lv=str->flag?ST_MIN_INT:ST_MAX_INT;
   if(L<=m){
      lv=_queryST_res(str,LeftC(k),L,R,l,m);      
   }
   if(R>m){
      rv=_queryST_res(str,RightC(k),L,R,m+1,r);      
   }
   return str->flag?(lv>rv?lv:rv):(lv>rv?rv:lv);
}

static void
_updateST_res(seg_tree_res_st * str,int k,int v,int L,int R,int l,int r){
   int m;

   if(((l-L)|(R-r))>=0){                       //见tlpi上笔记
      str->T[k]+=v;
      str->Lazy[k]+=v;
   }else{
       m=l+((r-l)>>1);
       _pushDown_res(str,k);
       if(L<=m){
          _updateST_res(str,LeftC(k),v,L,R,l,m);      
       }
       if(R>m){
          _updateST_res(str,RightC(k),v,L,R,m+1,r);      
       }
       _pushUp_res(str,k);
   }
}

seg_tree_res_st *
createST_res(int * nums,int numsSize,int flag){
   seg_tree_res_st * str;
   if(numsSize<1)
       return NULL;
   if((str=malloc(sizeof(seg_tree_res_st)))==NULL)
       return NULL;
   str->N=numsSize;
   str->flag=flag;
   if((str->T=malloc(sizeof(int)*3*numsSize))){  
       /*这一句是重中之重，我一开始以为和胜利树一样，非叶子是叶子数-1就可以了，，，也就是说size(T)=2*num就绰绰有余了,
        * 后面发现老是不对，，，发现是T太小了，有些元素没能装到，因为有些叶子节点尽管不在最底层，但仍然要空出两个儿子的位置给它,如【1，2，3】,共三层，第一层区间[0,2],第二层为[0,1],[2,2],第三层为[0,0],[0,1],[null],[null],需要七个节点。
       */
       if((str->Lazy=malloc(sizeof(int)*3*numsSize))==NULL){
           free(str->T);
           free(str);
           return NULL;
       }
   }else{
       free(str);
       return NULL;
   }
   _initST_res(str,nums,0,0,numsSize-1);
   return str;
}

int
queryST_res(seg_tree_res_st * str,int L,int R){
   return _queryST_res(str,0,L,R,0,str->N-1);     
}

void
updateST_res(seg_tree_res_st * str,int v,int L,int R){
   return _updateST_res(str,0,v,L,R,0,str->N-1);
}

void
destroyST_res(seg_tree_res_st * str){
   free(str->T);
   free(str->Lazy);
   free(str);
}





