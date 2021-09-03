#ifndef AES_ECB_HEAD
#define AES_ECB_HEAD

#include<string.h>

void print_box(unsigned char *,int );
void aes_encrypt(unsigned char * ,unsigned char * ,unsigned char * ,int );
void aes_decrypt(unsigned char * ,unsigned char * ,unsigned char * ,int );

#endif