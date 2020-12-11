#include"./RBTree.h"
#include<stdio.h>
int _error=0;
char _buf[1024];
void
checkRBT(struct RBTree * rbt,int index){
    _buf[index]=rbt->color?'B':'R';
    _buf[++index]='\0';

    if(rbt->left)
        checkRBT(rbt->left,index);
    else
        printf("%s\n",_buf);
    if(rbt->right)
        checkRBT(rbt->right,index);
    else
        printf("%s\n",_buf);
}

int 
main(){
  unsigned long test=0;
  struct RBTree *Root=NULL;
  int i=0;
 /* for(i=45;i<78;i++){
      printf("insert:%d->key:%d,value:%ld\n",insertRBT(&Root,i,i),i,i);
      test=i;   
  }
  for(i=25;i<38;i++){
      printf("insert:%d->key:%d,value:%ld\n",insertRBT(&Root,i,i),i,i);
  }
 */
  for(i=1;i<35;i++){
      printf("insert:%d->key:%d,value:%ld\n",insertRBT(&Root,i,i),i,i);
      test=i;   
  }
  for(i=80;i<110;i++){
      printf("insert:%d->key:%d,value:%ld\n",insertRBT(&Root,i,i),i,i);
      test=i;   
  }
  for(i=45;i<65;i++){
      printf("insert:%d->key:%d,value:%ld\n",insertRBT(&Root,i,i),i,i);
      test=i;   
  }

 /* for(i=0;i<110;i++){
      printf("search:key:%d,value:%ld->rt:%d\n",i,test,searchRBT(Root,i,&test));
  }
*/
 checkRBT(Root,0);
  for(i=110;i>1;i--){
      printf("delete:key:%d->rt:%d\n",i,deleteRBT(&Root,i));
  }
/*  for(i=0;i<110;i++){
      printf("search:key:%d,value:%ld->rt:%d\n",i,test,searchRBT(Root,i,&test));
  }
*/
 checkRBT(Root,0);

}
