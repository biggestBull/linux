#include"./ResRec.h"


struct _Test{
   struct _Test * left;
   struct _Test * right;
   int val;
   char c;
};

void
_recursive(struct _Test * test){
   if(test->left)
       _recursive(test->left);
   printf("%d",test->val);
   printf("%c\n",test->c);
   if(test->right)
       _recursive(test->right);
}

void
recursive(struct _Test * test){
   printf("%d",test->val);
   printf("%c\n",test->c);
   if(test->left)
       recursive(test->left);
   if(test->right)
       recursive(test->right);
}

main(){
  struct _Test test[10],*Root;
  int i;
  char buf_[1024],*buf;

  test[0].left=test+1;
  test[0].right=test+2;
  test[1].left=test+3;
  test[1].right=test+4;
  test[2].left=test+5;
  test[2].right=test+6;
  test[3].left=test+7;
  test[3].right=test+8;
  test[4].left=test+9;
  test[4].right=NULL;
  test[5].left=test[5].right=NULL;
  test[6].left=test[6].right=NULL;
  test[7].left=test[7].right=NULL;
  test[8].left=test[8].right=NULL;
  test[9].left=test[9].right=NULL;

  for(i=10;i<20;i++){
     test[i-10].val=i;
     test[i-10].c=i+97;
  }
  printf("rt:%d\n",restoreBinTree(test,sizeof(struct _Test),"_rr_test.b"));
  printf("rt:%p\n",Root=recoverBinTree(sizeof(struct _Test),"_rr_test.b"));
  _recursive(Root);
  printf("\n");
  recursive(Root);
}
