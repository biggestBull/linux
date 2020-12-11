#include"./SkewHeap.h"

static int
_SwapOrNot(struct SH_Node * H1,struct SH_Node *H2){               //我自己想的，真是妙啊。
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

static void
_swapChild(struct SH_Node * H){
   void * temp;
   temp=H->left;
   H->left=H->right;
   H->right=temp;
}

static struct SH_Node *
_merge(struct SH_Node * H1,struct SH_Node * H2){
   if(H1->left==NULL)
       H1->left=H2;
   else{
       H1->right=merge(H1->right,H2);
       _swapChild(H1);
   }
   return H1;
}



struct SH_Node *
merge(struct SH_Node * H1,struct SH_Node * H2){
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


struct SH_Node *
create_SH(enum SH_Type type,int flag,void * value,void * (*func)(void *)){
   struct SH_Node * SHN;

   if((SHN=malloc(sizeof(struct SH_Node)))==NULL)
       return NULL;
   SHN->left=SHN->right=NULL;
   SHN->type=type;
   SHN->value=value;
   SHN->flag=flag;
   SHN->func=func;

   return SHN;
}

int 
insert(struct SH_Node ** SHN,void * value){
   struct SH_Node * new_Node;

   if((new_Node=malloc(sizeof(struct SH_Node)))==NULL)
       return -1;
   memcpy(new_Node,*SHN,sizeof(struct SH_Node));
   new_Node->value=value;
   new_Node->left=new_Node->right=NULL;

   *SHN=merge(*SHN,new_Node);
   
   return 0;

}

int
pop(struct SH_Node ** SHN,void * rt_value,void * last_shn){
   struct SH_Node * left,*right;

   if(*SHN==NULL){
      return 1;     
   }
   if((*SHN)->func==NULL){
      switch((*SHN)->type){
         case CHAR:
             *(char *)rt_value=(char)((*SHN)->value);
             break;
         case INT:
             *(int *)rt_value=(int)((*SHN)->value);
             break;
         case LONG:
             *(long *)rt_value=(long)((*SHN)->value);
             break;
         default:
             return -1;
      }
   }else{
      *(unsigned long *)rt_value=(unsigned long)((*SHN)->value);      
   }
   left=(*SHN)->left;
   right=(*SHN)->right;
   if(left==right){
       (*SHN)->left=(*SHN)->right=NULL;
       *(unsigned long *)last_shn=(*SHN);
   }else{
       free((*SHN));     
   }
   *SHN=merge(left,right);
   return 0;

}


















