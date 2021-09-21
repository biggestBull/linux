#include"./RBTree.h"

static int                                      
_getColor(struct RBTree * rbt){
   return rbt?rbt->color:1;                      //red:0,black:1    
}

static struct RBTree *
_singleLeftRotate(struct RBTree * rbt,struct RBTree * * Root){
   struct RBTree * temp;

   temp=rbt->right;
 
   temp->parent=rbt->parent;
   rbt->parent=temp;
   if(temp->left)
       temp->left->parent=rbt;

   rbt->right=temp->left;
   temp->left=rbt;

   if(temp->parent==NULL)
       return *Root=temp;
   if(temp->parent->left==rbt)
       temp->parent->left=temp;
   else
       temp->parent->right=temp;
   return temp;
}

static struct RBTree *
_singleRightRotate(struct RBTree * rbt,struct RBTree ** Root){
   struct RBTree * temp;

   temp=rbt->left;
   
   temp->parent=rbt->parent;
   rbt->parent=temp;
   if(temp->right)
       temp->right->parent=rbt;

   rbt->left=temp->right;
   temp->right=rbt;
   if(temp->parent==NULL)
       return *Root=temp;
   if(temp->parent->left==rbt)
       temp->parent->left=temp;
   else
       temp->parent->right=temp;
   return temp;
}

static struct RBTree *
_doubleLeftRotate(struct RBTree * rbt,struct RBTree * * Root){
   _singleRightRotate(rbt->right,Root);
   return _singleLeftRotate(rbt,Root);
}

static struct RBTree *
_doubleRightRotate(struct RBTree * rbt,struct RBTree ** Root){
   _singleLeftRotate(rbt->left,Root);
   return _singleRightRotate(rbt,Root);
}


static struct RBTree *
_insert(struct RBTree * Root,int key,void * value){
   struct RBTree * temp=Root,*parent=Root?Root->parent:NULL;
   while(temp){
       parent=temp;
       if(temp->key>key){
           temp=temp->left;
       }
       else if(temp->key<key){
           temp=temp->right;
       }
       else{
           temp->value=value;
           return NULL;
       }
   }
   if((temp=malloc(sizeof(struct RBTree)))==NULL)
       return NULL;
   temp->color=0;
   temp->key=key;
   temp->value=value;
   temp->parent=parent;
   temp->left=temp->right=NULL;
    
   if(parent && parent->key>key)
       parent->left=temp;
   else if(parent && parent->key<key)
       parent->right=temp;
   return temp;

}

static int
_insertJudge(struct RBTree ** node){
   struct RBTree * father,* grandfather,*uncle;

   father=(*node)->parent;
   grandfather=father->parent;
   uncle=father==grandfather->left?grandfather->right:grandfather->left;
   if(!_getColor(uncle)){
       uncle->color=father->color=1;
       grandfather->color=0;
       *node=grandfather;
       return 0;    
   }
   if(grandfather->right==uncle){
       if(father->left==*node){
           *node=grandfather;
           return 1;               //左左 
       }else{
           *node=grandfather;
           return 2;                 //左右
       }
   }else{
       if(father->left==*node){
           *node=grandfather;
           return 3;                 //右左
       }
       else{
           *node=grandfather;
           return 4;                //右右
       }
   }
}

static struct RBTree *
_findnext(struct RBTree * Node){
   struct RBTree * temp=Node->right;

   while(temp->left){
       temp=temp->left;     
   }
   Node->value=temp->value;
   Node->key=temp->key;
   return temp;
}

static struct RBTree *
_findDeletedNode(struct RBTree * Root,int key){
   struct RBTree * temp=Root;

   while(temp){
      if(temp->key>key)
          temp=temp->left;
      else if(temp->key<key)
          temp=temp->right;
      else{
          if(temp->left && temp->right)
              return _findnext(temp);
          return temp;
      }
   }
   return NULL;
}

static int
_deleteJudge(struct RBTree * father,int l_or_r){         //l:0,r:1
   struct RBTree * brother;

   if(!l_or_r)
       brother=father->right;
   else
       brother=father->left;
   if(_getColor(brother) && _getColor(brother->left) && _getColor(brother->right)){
       brother->color=0;
       if(_getColor(father)==0){
           father->color=1;     
           return 0;                    //完成
       }
       return 1;                       //以father作当前节点     
   }
   if(_getColor(brother)==0){
       brother->color=1;
       father->color=0;
       return !l_or_r?2:3;                     //左旋|右旋
   }

   if(!l_or_r && _getColor(brother->left)==0 && _getColor(brother->right)){
       brother->color=0;
       brother->left->color=1;
       return 4;                     
   }

   if(l_or_r && _getColor(brother->left) && _getColor(brother->right)==0){
       brother->color=0;
       brother->right->color=1;
       return 5;                     
   }
   brother->color=father->color;
   father->color=1;                //最后一种。
   if(!l_or_r){
       brother->right->color=1;
       return 6;                         //左旋|右旋
   }
   brother->left->color=1;
   return 7;
}

int
insertRBT(struct RBTree ** Root,int key,void * value){
   struct RBTree * new_node;
   int flag=0;

   new_node=_insert(*Root,key,value);
   if(!new_node)
       return 1;
   if(!new_node->parent){
       *Root=new_node;
       new_node->color=1;
       return 0;
   }
   if(_getColor(new_node->parent)){
       return 0;
   }
   while(!flag){
       switch(flag=_insertJudge(&new_node)){
           case 0: 
               if(!new_node->parent){
                   new_node->color=1;
                   return 0;
               }else if(_getColor(new_node->parent)){
                   return 0;    
               }
               break;
          case 1:
               new_node=_singleRightRotate(new_node,Root);
               new_node->color=1;
               new_node->right->color=0;
               break;
          case 2:
               new_node=_doubleRightRotate(new_node,Root);
               new_node->color=1;
               new_node->right->color=0;
               break;
          case 3:
               new_node=_doubleLeftRotate(new_node,Root);
               new_node->color=1;
               new_node->left->color=0;
               break;
          case 4:
               new_node=_singleLeftRotate(new_node,Root);
               new_node->color=1;
               new_node->left->color=0;
               break;
       }
   }
   return 0;
}

int 
searchRBT(struct RBTree * Root,int key,void * rt){
   struct RBTree * temp=Root;

   while(temp){
      if(temp->key>key)
          temp=temp->left;
      else if(temp->key<key)
          temp=temp->right;
      else{
          *((unsigned long *)rt)=temp->value;  
          return 0;
      }
   }
   return 1;
}

int
deleteRBT(struct RBTree ** Root,int key){
   struct RBTree * del_node,*temp;
   int flag=1,l_or_r;

   del_node=_findDeletedNode(*Root,key);
   if(!del_node)
       return 1;
   if(!del_node->parent){
       (*Root)=(*Root)->left?(*Root)->left:(*Root)->right;   
       if(*Root){
           (*Root)->color=1;
           (*Root)->parent=NULL;
       }
       free(del_node);
       return 0;
   }
   temp=del_node->left?del_node->left:del_node->right;
   if(del_node->parent->left==del_node){
       del_node->parent->left=temp;
       l_or_r=0;
   }else{
       del_node->parent->right=temp;     
       l_or_r=1;
   }
   if(temp) 
       temp->parent=del_node->parent;
   
   if(_getColor(del_node)==0 ||  _getColor(temp)==0){
       if(temp){
           temp->color=1;
       }
       free(del_node);
       return 0;
   }
   temp=del_node->parent;
   free(del_node);
   while(flag){
       switch(flag=_deleteJudge(temp,l_or_r)){
           case 1:
               if(!temp->parent)
                   return 0;
               l_or_r=temp->parent->left==temp?0:1;
               temp=temp->parent;
               break;
           case 2:
               _singleLeftRotate(temp,Root);
               break;
           case 3:
               _singleRightRotate(temp,Root);
               break;
           case 4:
               _singleRightRotate(temp->right,Root);
               break;
           case 5:
               _singleLeftRotate(temp->left,Root);
               return 0;
           case 6:
               _singleLeftRotate(temp,Root);
               return 0;
           case 7:
               _singleRightRotate(temp,Root);
               return 0;
       }
   }
   return 0;
}



















