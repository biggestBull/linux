#include"./SplayTree.h"

static void * _temp_p;
static int _count;
static char _path;

static void * _rt_value;


static void
new_node(struct SplayTree_Node * Root,int key,void * value,int flag){
   Root->key=key;
   Root->value=value;
   Root->isRoot=0;
   if(flag!=0){
       Root->left=Root->right=NULL;     
   }
}

static struct SplayTree_Node *
right_rotate_reverse(struct SplayTree_Node * Node){
    struct SplayTree_Node * Node_son,*Node_grandson;
    Node_son=Node->left;
    Node_grandson=Node_son->left;

    Node->left=Node_son->right;
    Node_son->right=Node;
    Node_son->left=Node_grandson->right;
    Node_grandson->right=Node_son;

    return Node_grandson;
}


static struct SplayTree_Node *
left_rotate_reverse(struct SplayTree_Node * Node){
    struct SplayTree_Node * Node_son,*Node_grandson;
    Node_son=Node->right;
    Node_grandson=Node_son->right;

    Node->right=Node_son->left;
    Node_son->left=Node;
    Node_son->right=Node_grandson->left;
    Node_grandson->left=Node_son;

    return Node_grandson;
}


static struct SplayTree_Node *
left_rotate_single(struct SplayTree_Node * Node){
   struct SplayTree_Node * new_father;

   new_father=Node->right;
   Node->right=new_father->left;
   new_father->left=Node;

   return new_father;
}
    
static struct SplayTree_Node *
right_rotate_single(struct SplayTree_Node * Node){
   struct SplayTree_Node * new_father;

   new_father=Node->left;
   Node->left=new_father->right;
   new_father->right=Node;

   return new_father;
}

static struct SplayTree_Node *
left_rotate_double(struct SplayTree_Node * Node){
   Node->right=right_rotate_single(Node->right);
   return left_rotate_single(Node);
}

static struct SplayTree_Node *
right_rotate_double(struct SplayTree_Node * Node){
   Node->left=left_rotate_single(Node->left);
   return right_rotate_single(Node);
}

static struct SplayTree_Node *
_search_SplayT(struct SplayTree_Node * Root,int key){
   if(Root!=NULL && Root->isRoot!=-1){
       if(Root->key==key){
           _count=1;
           switch(Root->value_type){
              case CHAR:
                  *((char *)_rt_value)=(char)(Root->value);
                  break;
              case INT:
                  *((int *)_rt_value)=(int)(Root->value);
                  break;
              case LONG:
                  *((long *)_rt_value)=(long)(Root->value);
                  break;
              case PTR:
                  *((ul_ptr_t *)_rt_value)=(ul_ptr_t)(Root->value);
                  break;
              default:
                  return NULL;
           }
           printf("%disRoot:%d\n",Root->key,Root->isRoot);
           return Root;
       }else if(Root->key>key){
           _temp_p=_search_SplayT(Root->left,key);
           Root->left=_temp_p?_temp_p:Root->left;
       }else{
           _temp_p=_search_SplayT(Root->right,key);
           Root->right=_temp_p?_temp_p:Root->right;
       }

       if(_temp_p!=NULL){
           printf("%disRoot:%d\n",Root->key,Root->isRoot);
           _count++;
           if(_count==3){
               printf("3\n");
               _count=1;
             
               if(Root->key>key)
                   _path|=LEFT<<2;
               else
                   _path|=RIGHT<<2;
               printf("path:%d\n",_path);
               switch(_path){
                   case 5:
                       printf("LL\n");
                       if(Root->isRoot==1){
                           Root->isRoot=0;
                           Root->left->left->isRoot=1;
                       }
                       return right_rotate_reverse(Root);                //LL
                       break;         
                   case 9:            //LR
                       printf("LR\n");
                       if(Root->isRoot==1){
                           Root->isRoot=0;
                           Root->right->left->isRoot=1;
                       }
                       return left_rotate_double(Root);
                       break;                        
                   case 6:             //RL
                       printf("RL\n");
                       if(Root->isRoot==1){
                           Root->isRoot=0;
                           Root->left->right->isRoot=1;
                       }
                       return right_rotate_double(Root);
                       break;
                   case 10:            //RR
                       printf("RR\n");
                       if(Root->isRoot==1){
                           Root->isRoot=0;
                           Root->right->right->isRoot=1;
                       }
                       return left_rotate_reverse(Root);
                       break;
               }
               _path=0;
           }
           else{
               if(Root->key>key)
                   _path|=LEFT;
               else
                   _path|=RIGHT;
           }
       }else{
           return NULL;     
       }
   }
   return Root ;
}

int
search_SplayT(struct SplayTree_Node ** Root,int key,void * rt_value){
   struct SplayTree_Node * temp;
    
   _rt_value=rt_value;
    
   _path=0;
   if((temp=_search_SplayT(*Root,key))==NULL)
       return 1;
   if(_count==2){
       (*Root)->isRoot=0;
       if((*Root)->key>key){
           printf("rs\n");
           (*Root)->left->isRoot=1;
           (*Root)= right_rotate_single(*Root);
       }else{
           printf("ls\n");
           (*Root)->right->isRoot=1;
           (*Root)= left_rotate_single(*Root);
       }
       printf("%dnew Root:%d\n",(*Root)->key,(*Root)->isRoot);
   }else{
       *Root=temp;     
       printf("%dnew Root:%d\n",(*Root)->key,(*Root)->isRoot);
   }
   return 0;
}

struct SplayTree_Node *
create_SplayT(int key,void * value,enum Value_Type value_type){
   struct SplayTree_Node * Root;
   if((Root=malloc(sizeof(struct SplayTree_Node)))==NULL)
       return NULL;
   new_node(Root,key,value,1);
   Root->value_type=value_type;
   Root->isRoot=1;
   return Root;
}

int
insert_SplayT(struct SplayTree_Node * Root,int key,void * value){
   struct SplayTree_Node ** parent;
   enum Value_Type value_type;

   value_type=Root->value_type;
   
   if(Root->isRoot==-1){
       new_node(Root,key,value,0); 
       return 0;
   }

   while(Root!=NULL){
       if(Root->key==key){
           return 1;     
       }else if(Root->key>key){
           parent=&Root->left;
           Root=Root->left;
       }else{
           parent=&Root->right;
           Root=Root->right;
       }
   }
   if((Root=malloc(sizeof(struct SplayTree_Node )))==NULL)
       return -1;
   new_node(Root,key,value,1);
   Root->value_type=value_type;
   *parent=Root;
   
   return 0;
}


int
delete_SplayT(struct SplayTree_Node * Root,int key){
   struct SplayTree_Node ** parent;

   while(Root!=NULL){
       if(Root->key==key){
           struct SplayTree_Node * temp;
           if(Root->left && Root->right){
              
              temp=Root->right;
              parent=&Root->right;
              while(temp->left!=NULL){
                 parent=&temp->left;
                 temp=temp->left;    
              }
              new_node(Root,temp->key,temp->value,0);
              *parent=temp->right;
              free(temp);
           }else if(!Root->right && !Root->left){
               if(Root->isRoot==-1)
                   return 1;
               if(Root->isRoot==1){
                   Root->isRoot=-1;
               }else{
                   *parent=NULL;
                   free(Root);
               }
           }else{
               temp=Root->left?Root->left:Root->right;
               new_node(Root,temp->key,temp->value,0);
               Root->left=temp->left;
               Root->right=temp->right;
               free(temp);
           }
           return 0;
       }else if(Root->key>key){
           parent=&Root->left;
           Root=Root->left;
       }else{
           parent=&Root->right;
           Root=Root->right;
       }
   }

   return 1;
}



































