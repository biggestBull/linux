#include"./arrayTree.h"

/*
 * a为原数组，c为差值数组
 *
 * a[1]+a[2]+...+a[n]
 *
 * = (c[1]) + (c[1]+c[2]) + ... + (c[1]+c[2]+...+c[n]) 
 *
 * = n*c[1] + (n-1)*c[2] +... +c[n]
 *
 * = n * (c[1]+c[2]+...+c[n]) - (0*c[1]+1*c[2]+...+(n-1)*c[n])    (式子①)
 *
 * 那么我们就维护一个数组c2[n]，其中c2[i] = (i-1)*c[i] (即前i-1个数组a元素不贡献c[i]值)
 *
 * 每当修改c的时候，就同步修改一下c2，这样复杂度就不会改变
 *
 * 那么
 *
 * 式子①
 *
 * =n*sigma(c,n) - sigma(c2,n)
 *
 * 
 * 其原理其实就是“假设--排除”，假设区间【i，j】加上1：
 *       假设： n*sigma(c,n)-->n*c[i],即假设整个区间【1，n】都加上了1（以1为例，其它值也是同样的道理）。
 *       排除：- sigma(c2,n)-->(i-1)*c[i]，把前面的部分的加上的1给排除掉
 *   经过这两步，就变成了只有【i，n】加上了1了。
 *   但是，我们还需要排除掉【j+1，n】：
 *       和上同理，在j+1处开始减一就可以了（假设整个区间【1，n】都减去了1，然后排除【1，j】，
 *       这样就只减去了n-j个1），那么这样就会是数组c区间【i，j】各加上1，区间更新完成。
 * 
 * 于是我们做到了在O(logN)的时间内完成一次区间和查询
 * 原文链接：https://blog.csdn.net/weixin_41190227/article/details/82666855)))])]])])]])]]])]])]]]])]])])]]]
 * */

static int
_lowBit(int i){
   return i&(-i);        
}

static void
_update(array_tree_sum_st * ats,int i,int v){
   int x=i;

   for(;i<=ats->N;i+=_lowBit(i)){
       ats->Sum1[i]+=v;
       ats->Sum2[i]+=(x-1)*v;    
   }
}

static int
_getSum(array_tree_sum_st * ats,int i){
   int x=i,res=0;

   for(;i>0;i-=_lowBit(i)){
       res+=x*ats->Sum1[i]-ats->Sum2[i];     
   }
   return res;
}

array_tree_sum_st *
initAT(int * nums,int numsSize){
   int i;

   if(numsSize<1)
       return NULL;

   array_tree_sum_st * ats;
   if(ats=malloc(sizeof(array_tree_sum_st))){
        ats->N=numsSize;
        if(ats->Sum1=malloc(sizeof(int)*(numsSize+1))){
            if(ats->Sum2=malloc(sizeof(int)*(numsSize+1))){
               memset(ats->Sum1,0,sizeof(int)*(numsSize+1));        
               memset(ats->Sum2,0,sizeof(int)*(numsSize+1)); 
               for(i=1;i<=numsSize;i++){
                   if(i==1){
                       _update(ats,i,nums[0]);     
                   }else
                       _update(ats,i,nums[i-1]-nums[i-2]);     
               }
               return ats;
            }else{
               free(ats->Sum1);
               free(ats);
            }
        }else{
           free(ats);     
        }
   }
   return NULL;
}

void
updateAT(array_tree_sum_st * ats,int l,int r,int v){
   if(((l-1)|(ats->N-r))<0)
       return ;

    _update(ats,l,v);
    _update(ats,r+1,-v);
}

int
getSumAT(array_tree_sum_st * ats,int l,int r){
   if(((l-1)|(ats->N-r))<0)
       return 0;
   return _getSum(ats,r)-_getSum(ats,l-1);       
}

void
destroy(array_tree_sum_st * ats){
   free(ats->Sum1);
   free(ats->Sum2);
   free(ats);
}


