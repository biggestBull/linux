//for test
#include<stdio.h>
#include"aes_ecb.h"

int
main(){
    unsigned char secret[16*5+1],plain[16*5+1];
    int i;
    
    aes_encrypt("yanlo,world,you!hello,world,you!hello,world,you!hello,world,you!hello,world,you!","zccdefkhijolabcd",secret,5);
    aes_decrypt(secret,"zccdefkhijolabcd",plain,5);
    plain[16*5]='\0';
    printf("%s\n",plain);
 
    return 0;
}
