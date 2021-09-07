#ifndef AES_HEAD
#define AES_HEAD

#include<string.h>

#define AES_256                      //use AES_256 , default AES_128

extern void print_box(unsigned char box[256],int flag);

extern void aes_ecb_encrypt(unsigned char * plain,unsigned char * key,unsigned char * secret,int count);
extern void aes_ecb_decrypt(unsigned char * secret,unsigned char * key,unsigned char * plain,int count);

extern void aes_cbc_encrypt(unsigned char * plain,unsigned char * key,unsigned char * _vi,unsigned char * secret,int count);
extern void aes_cbc_decrypt(unsigned char * secret,unsigned char * key,char * _vi,unsigned char * plain,int count );

#endif
