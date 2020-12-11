#include<stdio.h>
#include"./segmentTree.h"

int main(){
    seg_tree_res_st * str;
    
    int nums[12]={
       1,2,3,4,5,6,7,8,9,10,11,12        
    },i;
    str=createST_res(nums,12,0);
    for(i=0;i<24;i++){
       printf("%d\n",str->T[i]);        
    }
    printf("\n\n");
    printf("[0,2] %d\n",queryST_res(str,0,2));
    printf("[4,7] %d\n",queryST_res(str,4,7));
    printf("[2,2] %d\n",queryST_res(str,2,2));
    printf("[11,12] %d\n",queryST_res(str,11,12));
    printf("[3,9] %d\n",queryST_res(str,3,9));

    printf("2,3,5\n10,1,5\n2,7,11\n");
    updateST_res(str,2,3,5);
    updateST_res(str,10,1,5);
    updateST_res(str,2,7,11);
    printf("[0,2] %d\n",queryST_res(str,0,2));
    printf("[4,7] %d\n",queryST_res(str,4,7));
    printf("[2,2] %d\n",queryST_res(str,2,2));
    printf("[11,12] %d\n",queryST_res(str,11,12));
    printf("[3,9] %d\n",queryST_res(str,3,9));
    
    printf("\n\n");
    updateST_res(str,2,6,6);
    printf("[0,2] %d\n",queryST_res(str,0,2));
    printf("[4,7] %d\n",queryST_res(str,4,7));
    printf("[2,2] %d\n",queryST_res(str,2,2));
    printf("[11,12] %d\n",queryST_res(str,11,12));
    printf("[3,9] %d\n",queryST_res(str,3,9));
}
