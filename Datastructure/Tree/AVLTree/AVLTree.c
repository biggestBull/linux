#include"./AVLTree.h"

static int 
get_height(struct AVLTree_Node * Node){
   if(Node==NULL)
       return -1;
   return Node->height;
}

static struct AVLTree_Node *
left_rotate_single(struct AVLTree_Node * Node){
   struct AVLTree_Node * new_father;

   new_father=Node->right;
   Node->right=new_father->left;
   new_father->left=Node;

   Node->height=get_height(Node->left)>get_height(Node->right)?get_height(Node->left)+1:get_height(Node->right)+1;
   new_father->height=get_height(new_father->left)>get_height(new_father->right)?get_height(new_father->left)+1:get_height(new_father->right)+1;

   return new_father;
}
    
static struct AVLTree_Node *
right_rotate_single(struct AVLTree_Node * Node){
   struct AVLTree_Node * new_father;

   new_father=Node->left;
   Node->left=new_father->right;
   new_father->right=Node;

   Node->height=get_height(Node->left)>get_height(Node->right)?get_height(Node->left)+1:get_height(Node->right)+1;
   new_father->height=get_height(new_father->left)>get_height(new_father->right)?get_height(new_father->left)+1:get_height(new_father->right)+1;

   return new_father;
}

static struct AVLTree_Node *
left_rotate_double(struct AVLTree_Node * Node){
   Node->right=right_rotate_single(Node->right);
   return left_rotate_single(Node);
}

static struct AVLTree_Node *
right_rotate_double(struct AVLTree_Node * Node){
   Node->left=left_rotate_single(Node->left);
   return right_rotate_single(Node);
}

static void
new_node(struct AVLTree_Node * Node,key_t key,void * value,int flag){
   Node->key=key;
   Node->value=value;
   Node->height=0;
   if(flag!=0)
      Node->left=Node->right=NULL;
}

static struct AVLTree_Node * 
_insert_node(struct AVLTree_Node * Root,key_t key,void * value){
   int left_height,right_height;
   void * _temp_p;

   if(Root!=NULL){
      if(Root->key>key){
         _temp_p= _insert_node(Root->left,key,value);
         Root->left=_temp_p?_temp_p:Root->left;
      }
      else if(Root->key<key){
           _temp_p= _insert_node(Root->right,key,value);   
           Root->right=_temp_p?_temp_p:Root->right;
      }else{
		 Root->value=value;
         return Root;    
      }
      if(_temp_p!=NULL){
         left_height=get_height(Root->left);
         right_height=get_height(Root->right);
         Root->height=left_height>right_height?left_height+1:right_height+1;

         if(left_height>right_height+1){
            if(Root->left->key>key){
               Root= right_rotate_single(Root);       
            }else{
               Root= right_rotate_double(Root);        
            }
         }else if(right_height>left_height+1){
            if(Root->right->key<key){
               Root= left_rotate_single(Root);       
            }else{
               Root= left_rotate_double(Root);        
            }
         }

      }else{
         return NULL;      
      }

   }else{
      if((Root=malloc(sizeof(struct AVLTree_Node )))==NULL){
          return NULL;      
      }
      new_node(Root,key,value,1);      
   }
   return Root;
}

static struct AVLTree_Node *
_cover_by_Rmin(struct AVLTree_Node * dest,struct AVLTree_Node * Root){
    int left_height,right_height;
	void * _temp_p;
    
    if(Root->left==NULL){
       _temp_p=Root->right;
       new_node(dest,Root->key,Root->value,0);
       free(Root);
       return _temp_p;
    }
    Root->left=_cover_by_Rmin(dest,Root->left);
    left_height=get_height(Root->left);
    right_height=get_height(Root->right);
    Root->height=left_height>right_height?left_height+1:right_height+1;

   /*    if(left_height>right_height+1){
          if(Root->left->key>key){
             Root=right_rotate_single(Root);       
          }else{
             Root= right_rotate_double(Root);        
          }
       }else */
    if(right_height>left_height+1){
        if(get_height(Root->right->right)>=get_height(Root->right->left)){
            Root=left_rotate_single(Root);       
        }else{
           Root= left_rotate_double(Root);        
        }
    }
    return Root;
}

static struct AVLTree_Node *
_delete_AVLT(struct AVLTree_Node * Root,key_t key){
   int left_height,right_height;
   void * _temp_p;

   if(Root!=NULL){
       if(Root->key==key){
          if(Root->left && Root->right){
              Root->right=_cover_by_Rmin(Root,Root->right);   
          }else if(!Root->left && !Root->right){
              free(Root);
              return NULL;
          }else{
              _temp_p=Root->left?Root->left:Root->right;
              free(Root);
              return _temp_p;
          }

       }else if(Root->key>key){
          Root->left=_delete_AVLT(Root->left,key);      
       }else{
         Root->right =_delete_AVLT(Root->right,key);      
       }
      
       left_height=get_height(Root->left);
       right_height=get_height(Root->right);
       Root->height=left_height>right_height?left_height+1:right_height+1;

       if(left_height>right_height+1){
          if(get_height(Root->left->left)>=get_height(Root->left->right)){
             Root=right_rotate_single(Root);       
          }else{
             Root= right_rotate_double(Root);        
          }
       }else if(right_height>left_height+1){
          if(get_height(Root->right->right)>=get_height(Root->right->left)){
              Root=left_rotate_single(Root);       
          }else{
             Root= left_rotate_double(Root);        
          }
       }
   }
            
   return Root; 
}



struct AVLTree_ST *
create_AVLT(enum Value_Type value_type){
   struct AVLTree_ST * AVLT;

   if((AVLT=malloc(sizeof(struct AVLTree_ST)))==NULL)
       return NULL;
   AVLT->value_type=value_type;
   AVLT->Root=NULL;

   return AVLT;
}

int
search_AVLT(struct AVLTree_ST * AVLT,key_t key,void * rt_value){
   enum Value_Type _value_type=AVLT->value_type;
   struct AVLTree_Node * Root=AVLT->Root;

   while(Root!=NULL){
       if(Root->key==key){
          switch(_value_type){
              case CHAR:
                  *((char *)rt_value)=(char)(Root->value);
                  break;
              case INT:
                  *((int *)rt_value)=(int)(Root->value);
                  break;
              case LONG:
                  *((long *)rt_value)=(long)(Root->value);
                  break;
              case PTR:
                  *((ul_ptr_t *)rt_value)=(ul_ptr_t)(Root->value);
                  break;
              default:
                  return -1;
          }
          return 0;
       }
       if(Root->key>key)
           Root=Root->left;
       else
           Root=Root->right;
   }
   return 1;
}

struct AVLTree_Node *
insert_AVLT(struct AVLTree_ST * AVLT,key_t key,void * value){
   struct AVLTree_Node * Root;

   Root = _insert_node(AVLT->Root,key,value);
   if(Root){
	  AVLT->Root=Root;
   }
   return Root;                                            //插入是否成功（更新属于插入成功）
}

struct AVLTree_Node *
delete_AVLT(struct AVLTree_ST * AVLT,key_t key){
	AVLT->Root=_delete_AVLT(AVLT->Root,key);
    return AVLT->Root;                                     //经删除后是否还有节点
}

void
static _free_Nodes(struct AVLTree_Node * Root){
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

void
free_AVLT(struct AVLTree_ST * AVLT){
	_free_Nodes(AVLT->Root);
	free(AVLT);
}





























