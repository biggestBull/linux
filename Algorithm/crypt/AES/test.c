//for test
#include<stdio.h>
#include"aes.h"

int
main(){
    unsigned char secret[16*5+1],plain[16*5+1];
    int i;
    
    aes_cbc_encrypt("你lo,world,you!hello,world,you!hello,world,you!","你defkhijolabcd","abcdefghijklmnop",secret,3);
    aes_cbc_decrypt(secret,"你defkhijolabcd","abcdefghijklmnop",plain,3);
    plain[16*3]='\0';
    printf("%s\n",plain);
 
    return 0;
}
