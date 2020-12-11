#include<stdlib.h>
#include<string.h>

typedef struct {
  int N;
  int * Sum1,*Sum2;
}array_tree_sum_st;

array_tree_sum_st *
initAT(int * nums,int numsSize);
void
updateAT(array_tree_sum_st * ats,int l,int r,int v);
int
getSumAT(array_tree_sum_st * ats,int l,int r);
void
destroy(array_tree_sum_st * ats);
