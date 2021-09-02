#include"./Algorithm.h"

int                                    //if true,return index of the string
_KMP(const char * string,const char * key_string,int * fail_arr){
    int i,j,rt_index=-1,string_len,key_len;
    
    i=-1,j=0,fail_arr[0]=-1;
    key_len=strlen(key_string);
    for(;j<key_len-1;j++){
       while(i>=0 && key_string[i+1] != key_string[j+1]) i=fail_arr[i];
       if(key_string[i+1]==key_string[j+1]){
           fail_arr[j+1]=++i;
       }else
           fail_arr[j+1]=-1;
    }
    string_len=strlen(string);
    for(i=0,j=-1;i<string_len;i++){
       while(j>=0 && string[i]!=key_string[j+1]) {
           j=fail_arr[j];     
       }
       if(string[i]==key_string[j+1]){
           if(j==-1)
               rt_index=i+1;
           j++;

           if(j+1==key_len)
               return rt_index;
       }
       
    }
    return -1;
}
