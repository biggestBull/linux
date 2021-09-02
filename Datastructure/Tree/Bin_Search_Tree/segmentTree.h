#include<string.h>
#include<stdlib.h>

typedef struct{
   int N,flag;
   int *T,*Lazy;

}seg_tree_res_st;   

seg_tree_res_st *
createST_res(int * nums,int numsSize,int flag);
int
queryST_res(seg_tree_res_st * str,int L,int R);
void
updateST_res(seg_tree_res_st * str,int v,int L,int R);
void
destroyST_res(seg_tree_res_st * str);
