
#ifndef DJSET_H
#define DJSET_H
#include<stdlib.h>

int * 
DJSetInit(int size);
int
DJFind(int * DJSet,int X);
void
DJUnion(int * DJSet,int X,int Y);

#endif    
