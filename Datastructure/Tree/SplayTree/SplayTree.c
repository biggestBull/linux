#include"./SplayTree.h"

static void
new_node(struct SplayTree_Node * Root,key_t key,void * value,int flag){
   Root->key=key;
   Root->value=value;
   if(flag)
      Root->left=Root->right=NULL;     
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
_search_SplayT(struct SplayTree_Node * Root,enum Value_Type value_type,key_t key,void * _rt_value,int * _count,char * _path){
   void * _temp_p;
   
   if(Root!=NULL){
       if(Root->key==key){
	   printf("==key\n");
           *_count=1;
           switch(value_type){
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
           }
           return Root;
       }else if(Root->key>key){
           _temp_p=_search_SplayT(Root->left,value_type,key,_rt_value,_count,_path);
           Root->left=_temp_p?_temp_p:Root->left;
       }else{
           _temp_p=_search_SplayT(Root->right,value_type,key,_rt_value,_count,_path);
           Root->right=_temp_p?_temp_p:Root->right;
       }

       printf("_temp_p:%d\n",Root->key);
       *_count+=1;

       if(*_count==3){
          printf("3\n");
          *_count=1;
             
          if(Root->key>key)
              *_path|=LEFT<<2;
          else
              *_path|=RIGHT<<2;
          printf("path:%d\n",*_path);
          switch(*_path){
             case 5:
                printf("LL\n");
                return right_rotate_reverse(Root);   //LL
             case 9:            //LR
                printf("LR\n");
                return left_rotate_double(Root);
             case 6:             //RL
                printf("RL\n");
                return right_rotate_double(Root);
             case 10:            //RR
                printf("RR\n");
                return left_rotate_reverse(Root);
           }
              *_path=0;
       }else if(*_count==2){
           if(Root->key>key)
               *_path|=LEFT;
           else
               *_path|=RIGHT;
       }
   }

   return Root;
}

void
static _free_Nodes(struct SplayTree_Node * Root){
	if(Root){
		if(Root->left){
			_free_Nodes(Root->left);
		}
		if(Root->right){
			_free_Nodes(Root->right);
		}
		free(Root);
	}
}

struct SplayTree_ST *
create_SplayT(key_t key,enum Value_Type value_type){
   struct SplayTree_ST * SplayT;

   if((SplayT=malloc(sizeof(struct SplayTree_ST)))==NULL)
       return NULL;
   SplayT->value_type=value_type;
   SplayT->Root=NULL;

   return SplayT;
}

void
free_SplayT(struct SplayTree_ST * SplayT){
	_free_Nodes(SplayT->Root);
	free(SplayT);
}

int
search_SplayT(struct SplayTree_ST * SplayT,key_t key,void * rt_value){
   struct SplayTree_Node * temp;
   int _count=0;
   char _path=0;

   temp=_search_SplayT(SplayT->Root,SplayT->value_type,key,rt_value,&_count,&_path);
   if(temp==NULL)
       return 2;

   if(_count==2){
       if(SplayT->Root->key>key){
           printf("rs\n");
           SplayT->Root= right_rotate_single(SplayT->Root);
       }else{
           printf("ls\n");
           SplayT->Root= left_rotate_single(SplayT->Root);
       }
       printf("%dnew Root\n",SplayT->Root->key);
   }else{
       SplayT->Root=temp;     
       printf("%dnew Root\n",SplayT->Root->key);
   }

   if(SplayT->Root->key!=key)
	   return 1;
   return 0;
}

int
insert_SplayT(struct SplayTree_ST * SplayT,key_t key,void * value){
   struct SplayTree_Node ** parent,*Root=SplayT->Root;
   
   while(Root!=NULL){
       if(Root->key==key){
           new_node(Root,key,value,0);
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
   if(SplayT->Root==NULL)
       SplayT->Root=Root;
   else
       *parent=Root;

   return 0;
}

int
delete_SplayT(struct SplayTree_ST * SplayT,key_t key){
   struct SplayTree_Node ** parent,*temp,*Root=SplayT->Root;

   while(Root!=NULL){
       if(Root->key==key){
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
               if(Root==SplayT->Root){
                   SplayT->Root=NULL;
               }else{
                   *parent=NULL;
               }
               free(Root);
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


