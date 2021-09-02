#include"./LeftHeap.h"

static int
_SwapOrNot(struct LH_Node * H1,struct LH_Node *H2){               //我自己想的，真是妙啊。
   void * a,*b;
   int rt;
   
   a=H1->func==NULL?H1->value:(*(H1->func))(H1->value);
   b=H1->func==NULL?H2->value:(*(H1->func))(H2->value);
   if(H1->flag){
      switch(H1->type){
          case CHAR:
              rt=(char)a>=(char)b?0:1;
              break;
          case INT:
              rt=(int)a>=(int)b?0:1;
              break;
          case LONG:
              rt=(long)a>=(long)b?0:1;
              break;
          default:
              return -1;             //应当致命错误，不能返回给merge，会乱套
      }
   }else{
      switch(H1->type){
          case CHAR:
              rt=(char)a<=(char)b?0:1;
              break;
          case INT:
              rt=(int)a<=(int)b?0:1;
              break;
          case LONG:
              rt=(long)a<=(long)b?0:1;
              break;
          default:
              return -1;
      }
            
   }
   return rt;
}

static int
_getNPL(struct LH_Node * lhn){             //这个函数并没有什么必要，因为在_merge的else中其左右儿子不可能是NULL
  if(lhn==NULL)
      return -1;
  return lhn->npl;
}

static void
_swapChild(struct LH_Node * H){
   void * temp;
   temp=H->left;
   H->left=H->right;
   H->right=temp;
}

static struct LH_Node *
_merge(struct LH_Node * H1,struct LH_Node * H2){
   if(H1->left==NULL)
       H1->left=H2;
   else{
       H1->right=merge(H1->right,H2);
       if(_getNPL(H1->left)<_getNPL(H1->right))
           _swapChild(H1);
       H1->npl=_getNPL(H1->right)+1;
   }
   return H1;
}



struct LH_Node *
merge(struct LH_Node * H1,struct LH_Node * H2){
   if(H1==NULL)
       return H2;
   if(H2==NULL)
       return H1;
   if(_SwapOrNot(H1,H2))
       return _merge(H2,H1);
   else{
       return _merge(H1,H2);     
   }
}


struct LH_Node *
create_LH(enum LH_Type type,int flag,void * value,void * (*func)(void *)){
   struct LH_Node * LHN;

   if((LHN=malloc(sizeof(struct LH_Node)))==NULL)
       return NULL;
   LHN->left=LHN->right=NULL;
   LHN->type=type;
   LHN->npl=0;
   LHN->value=value;
   LHN->flag=flag;
   LHN->func=func;

   return LHN;
}

int 
insert(struct LH_Node ** LHN,void * value){
   struct LH_Node * new_Node;

   if((new_Node=malloc(sizeof(struct LH_Node)))==NULL)
       return -1;
   memcpy(new_Node,*LHN,sizeof(struct LH_Node));
   new_Node->value=value;
   new_Node->left=new_Node->right=NULL;
   new_Node->npl=0;

   *LHN=merge(*LHN,new_Node);
   
   return 0;

}

int
pop(struct LH_Node ** LHN,void * rt_value,void * last_lhn){
   struct LH_Node * left,*right;

   if(*LHN==NULL){
      return 1;     
   }
   if((*LHN)->func==NULL){
      switch((*LHN)->type){
         case CHAR:
             *(char *)rt_value=(char)((*LHN)->value);
             break;
         case INT:
             *(int *)rt_value=(int)((*LHN)->value);
             break;
         case LONG:
             *(long *)rt_value=(long)((*LHN)->value);
             break;
         default:
             return -1;
      }
   }else{
      *(unsigned long *)rt_value=(unsigned long)((*LHN)->value);      
   }
   left=(*LHN)->left;
   right=(*LHN)->right;
   if(left==right){
       (*LHN)->left=(*LHN)->right=NULL;
       *(unsigned long *)last_lhn=(*LHN);
   }else{
       free((*LHN));     
   }
   *LHN=merge(left,right);
   return 0;

}


















