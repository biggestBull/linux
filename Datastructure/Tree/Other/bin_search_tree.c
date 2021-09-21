#include"./bin_search_tree.h"

struct BinTree_Node * 
create_BinTree(int key,ul_ptr_t value,enum Value_type value_type){
   struct BinTree_Node * Root; 
   if((Root=malloc(sizeof(struct BinTree_Node)))==NULL)
       return NULL;
   Root->key=key;
   Root->value=(void *)value;
   Root->value_type=value_type;
   Root->left=Root->right=NULL;
   Root->flag=1;

   return Root;
}

int
search_BinTree(struct BinTree_Node * Root,int key,void * rt_value){
    enum Value_type value_type=Root->value_type;
    
    if(Root->flag==0)
        return 1;

    while(Root!=NULL){
       if(Root->key==key){
           switch(value_type){
               case CHAR:
                  *((char * )rt_value)=(char)Root->value;
                   break;
               case INT:
                   *((int * )rt_value)=(int)Root->value;
                   break;
               case LONG:
                   *((long * )rt_value)=(long)Root->value;
                   break;
               case PTR:                               //should use ul_ptr_t
                   *((ul_ptr_t * )rt_value)=(ul_ptr_t)Root->value;
                   break;
               default:
                   return -1;
           }
           return 0;
       }
       if(Root->key>key)
           Root=Root->left;
       if(Root->key<key)
           Root=Root->right;
   }
   return 1;
}

int
insert_BinTree(struct BinTree_Node * Root,int key,ul_ptr_t value){
   struct BinTree_Node ** parent=NULL;
    
   if(Root->value_type==-1){
       return -1;      
   }
   if(Root->flag==0){
      Root->key=key;
      Root->value=(void *)value;
      Root->flag=1;
      return 0;
   }
   while(Root!=NULL){
       if(Root->key==key){
           return 1;
       }
       
       if(Root->key>key){
           parent=&Root->left;     
           Root=Root->left;
       }
       else{
           parent=&Root->right;     
           Root=Root->right;
       }
   }
   if((Root=malloc(sizeof(struct BinTree_Node)))==NULL)
       return -1;
   Root->key=key;
   Root->value=(void *)value;
   Root->value_type=-1;
   Root->flag=1;
   Root->left=Root->right=NULL;
   *parent=Root;
  
   return 0;
}


int
delete_BinTree(struct BinTree_Node * Root,int key){
   struct BinTree_Node ** parent=NULL;
    
    while(Root!=NULL){
       if(Root->key>key){
           parent=&Root->left;     
           Root=Root->left;
       }
       else if(Root->key<key){
           parent=&Root->right;     
           Root=Root->right;
       }
       else{
           struct BinTree_Node * temp;
          
           if(Root->left==NULL && Root->right==NULL){
               if(Root->value_type!=-1)
                    return Root->flag=0;
               *parent=NULL;
               free(Root);
           }else if(Root->left && Root->right){
               
               parent=&Root->right;
               temp=Root->right;
               while(temp->left!=NULL){
                   parent=&temp->left;
                   temp=temp->left;     
               }
               Root->key=temp->key;
               Root->value=temp->value;
               *parent=temp->right;
               free(temp);
           }else{
               temp=Root->left?Root->left:Root->right;
               Root->key=temp->key;
               Root->value=temp->key;
               Root->left=temp->left;
               Root->right=temp->right;
               free(temp);
            /*   if(Root->value_type!=-1){
                   parent=Root->left?&Root->left:&Root->right;
                
                   Root->key=(*parent)->key;
                   Root->value=(*parent)->value;
                   Root->value_type=(*parent)->value_type;
                   Root->left=(*parent)->left;
                   Root->right=(*parent)->right;
                   free(*parent);
                   return 0;     
               }
               *parent=Root->left?Root->left:Root->right;
               (*parent)->value_type=Root->value_type;
               free(Root);
               */
           }
           return 0;
       }
    }
    return 1;                                     //is empty
}













