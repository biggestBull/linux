
#ifndef RESREC_H
#define RESREC_H

#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>

typedef struct General_ST{
   struct General_ST * l;
   struct General_ST * r;
}general_st;

int 
restoreBinTree(general_st * gst,int size,char * filename);
general_st *
recoverBinTree(int size,char * filename);

#endif
