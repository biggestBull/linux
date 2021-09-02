#include"./DoubleHash.h"

 int
plusCap(struct DH_Table * DHT);

int
main(int argc,char * argv[]){
    struct DH_Table * DHT;
    unsigned long value;
    char ch[]={
       'e',149,'\0'        
    };
    char ch2[]={
       'd',181,'\0'        
    };

    DHT=init_DHTable(PTR);
    printf("开始\n");
    printf("containkey:%s:%d\n","fu",DHT_containKey(DHT,"fu"));
    printf("get:%s:value:%ld:%d\n","fu",value,DHT_get(DHT,"fu",&value));
    printf("insert:%s:%d\n","fu",DHT_insert(DHT,"fu",123));
    printf("insert:%s:%d\n","fu",DHT_insert(DHT,"fu",123));
    printf("insert:%s:%d\n",ch,DHT_insert(DHT,ch,12));
    printf("insert:%s:%d\n",ch2,DHT_insert(DHT,ch2,12));
    printf("get:%s:value:%ld:%d\n","fu",value,DHT_get(DHT,"fu",&value));
    printf("get:%s:value:%ld:%d\n",ch,value,DHT_get(DHT,ch,&value));
    printf("get:%s:value:%ld:%d\n",ch2,value,DHT_get(DHT,ch2,&value));
    printf("delete:%s:%d\n",ch,DHT_delete(DHT,ch));
    printf("get:%s:value:%ld:%d\n","fu",value,DHT_get(DHT,"fu",&value));
    printf("get:%s:%d\n",ch,DHT_get(DHT,ch,&value));

    plusCap(DHT);
    printf("开始\n");
    printf("containkey:%s:%d\n","fu",DHT_containKey(DHT,"fu"));
    printf("get:%s:value:%ld:%d\n","fu",value,DHT_get(DHT,"fu",&value));
    printf("insert:%s:%d\n","fu",DHT_insert(DHT,"fu",123));
    printf("insert:%s:%d\n","fu",DHT_insert(DHT,"fu",123));
    printf("insert:%s:%d\n",ch,DHT_insert(DHT,ch,12));
    printf("insert:%s:%d\n",ch2,DHT_insert(DHT,ch2,12));
    printf("get:%s:value:%ld:%d\n","fu",value,DHT_get(DHT,"fu",&value));
    printf("get:%s:value:%ld:%d\n",ch,value,DHT_get(DHT,ch,&value));
    printf("get:%s:value:%ld:%d\n",ch2,value,DHT_get(DHT,ch2,&value));
    printf("delete:%s:%d\n",ch,DHT_delete(DHT,ch));
    printf("get:%s:value:%ld:%d\n","fu",value,DHT_get(DHT,"fu",&value));
    printf("get:%s:%d\n",ch,DHT_get(DHT,ch,&value));
}
