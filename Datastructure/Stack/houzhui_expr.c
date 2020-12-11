#include"./stack.h"

struct Opt_symbol{
   int level;
   char symbol;
};

static int
compare_ele(struct Stack * stack,struct Opt_symbol * opt_symbol){
   int status;
   unsigned long ptr;
   struct Opt_symbol * opt_symbol_temp;
   while((status=stack_pop(stack,&ptr))>=0){
        opt_symbol_temp=ptr;
        if(opt_symbol_temp->level<opt_symbol->level){
            if(stack_push(stack,opt_symbol_temp)<0){
                printf("push failed!\n");
                exit(1);
            }
            if(stack_push(stack,opt_symbol)<0){
                printf("push failed!\n");
                exit(1);
            }
            return 0;
        }
        printf("%c",opt_symbol_temp->symbol);

   }
   if(status==-1){
       if(stack_push(stack,opt_symbol)<0){
           printf("push failed\n ");
           exit(1);
       }
       return 0;
   }else{
       printf("pop failed \n");
       exit(1);
   }
}

int
main(int argc,char * argv[]){
   char expr[1024];
   int i=0;
   struct Stack stack;
   struct Opt_symbol plus,sub,mult,div,left;
   struct Opt_symbol * temp;
   unsigned long ptr;
   int status;
   
   left.level=0;
   plus.level=sub.level=1;
   mult.level=div.level=2;
   plus.symbol='+';
   sub.symbol='-';
   mult.symbol='*';
   div.symbol='/';

   if(create_stack(&stack,PTR)==-1){
       printf("create stack failed!\n");
       exit(1);
   }

   scanf("%s",expr);
   
   while(expr[i]){
       switch(expr[i]){
           case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
               printf("%c",expr[i]);
               break;
           case '-':
               compare_ele(&stack,&sub);
               break;
           case '+':
               compare_ele(&stack,&plus);
               break;
           case '*':
               compare_ele(&stack,&mult);
               break;
           case '/':
               compare_ele(&stack,&div);
               break;
           case '(':
               if(stack_push(&stack,&left)<0){
                  printf("push failed!\n"); 
                  exit(1);
               }
               break;
           case ')':
               while((status=stack_pop(&stack,&ptr))>=0){
                   temp=ptr;
                   if(temp->level==0)
                       break;
                   printf("%c",temp->symbol);
               }
               if(status<0){
                   printf("synax error!\n");
                   exit(1);
               }
               break;
           default:
               printf("%c:what fuck?\n",expr[i]);
       }
       i++;
   }
   while((status=stack_pop(&stack,&ptr))>=0){
       temp=ptr;
       printf("%c",temp->symbol);
  }
  if(status!=-1)
      printf("pop failed!\n");
  printf("\n");
}
