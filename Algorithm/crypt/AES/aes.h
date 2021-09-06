#ifndef AES_HEAD
#define AES_HEAD

#include<string.h>

void print_box(unsigned char *,int );

void aes_ecb_encrypt(unsigned char * ,unsigned char * ,unsigned char * ,int );
void aes_ecb_decrypt(unsigned char * ,unsigned char * ,unsigned char * ,int );

void aes_cbc_encrypt(unsigned char * ,unsigned char * ,unsigned char *,unsigned char * ,int );
void aes_cbc_decrypt(unsigned char * ,unsigned char * ,unsigned char *,unsigned char * ,int );

#endif
