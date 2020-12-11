
#ifndef SORT_HEAD
#define SORT_HEAD
#include<stdio.h>
#include<stdlib.h>

void
shell_sort(int * arr,int length,int flag);
void
quick_sort(int * arr,int left,int right,int flag);
void
heap_sort(int * arr,int length,int flag);
int
merge_sort(int * arr,int length,int flag);
int
external_sort(int fd,int flag,int memsize,int num,char interval_char);


#endif    
