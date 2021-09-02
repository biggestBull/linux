#ifndef HFM_HEAD
#define HFM_HEAD

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include"../../data_structure/Heap/Heap.h"

#define HFM_BUFFER_SIZE 1024 

struct Huffman_Node{
   long count;
   char c;
   struct Huffman_Node * left;
   struct Huffman_Node * right;
};

int
Huffman_compress(const char * filepath,const char * outfilepath,int flag);
int
Huffman_decompress(const char * filepath,const char * outfilepath,int flag);
#endif

