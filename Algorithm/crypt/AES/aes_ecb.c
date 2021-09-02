#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*
sbox:
                                         0x63
low_bit          ——
            |    1 0 0 0 1 1 1 1           1
            |    1 1 0 0 0 1 1 1           1   
            |    1 1 1 0 0 0 1 1           0 
            |    1 1 1 1 0 0 0 1           0
            |    1 1 1 1 1 0 0 0           0
            |    0 1 1 1 1 1 0 0           1
            |    0 0 1 1 1 1 1 0           1 
            |    0 0 0 1 1 1 1 1           0
hight_bit        ——

---------------------------------------没毛病

is_box:
                                          0xA0
low_bit         —— 
            |   0 1 0 1 0 0 1 0            0
            |   0 0 1 0 1 0 0 1            0
            |   1 0 0 1 0 1 0 0            0
            |   0 1 0 0 1 0 1 0            0
            |   0 0 1 0 0 1 0 1            0
            |   1 0 0 1 0 0 1 0            1
            |   0 1 0 0 1 0 0 1            0
            |   1 0 1 0 0 1 0 0            1
hight_bit       ——

------------------------------------------有毛病
这个是参考知乎上那篇文章，但我发现算出来的结果老不对，参考了一下别人的，发现
用的异或的数字是0x05（这说明这个作者显然是把顺序写反了（图中是高位低字节，
低位高字节）），至于左乘矩阵是什么，暂时还不知道，但可以参考这段代码，管中窥豹：

for(i=0;i<8;i++){                   
        result^=(((input>>(i+2)%8)&1)^((input>>(i+5)%8)&1)^((input>>(i+7)%8)&1))<<i;
    }  
故猜测矩阵应该是这样才对：

is_box:
                                          0x05
low_bit         —— 
            |   0 0 1 0 0 1 0 1            1
            |   1 0 0 1 0 0 1 0            0
            |   0 1 0 0 1 0 0 1            1
            |   1 0 1 0 0 1 0 0            0
            |   0 1 0 1 0 0 1 0            0
            |   0 0 1 0 1 0 0 1            0
            |   1 0 0 1 0 1 0 0            0
            |   0 1 0 0 1 0 1 0            0
hight_bit       ——

破案了：
我再对比了一下，它这矩阵就和那个异或的数字一样，也是反的，，，矩阵水平
和垂直翻转一下就得到了下面这个真正的矩阵！！！


 const unsigned char S_Table[256] =
{
0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

const unsigned char ReS_Table[256] =
{
0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

*/
static int
_get_hightest_position(unsigned short Number)
{
    int i = 0;
    while (Number)         
    {
        i++;
        Number = Number >> 1;
    }
    return i;
}

static unsigned char
_division(unsigned short Num_L, unsigned short Num_R, unsigned short *Remainder)
{
    //NUM_L / NUM_R  , 代码还是很好理解的
    
    unsigned char q = 0;
    int bitCount = 0;
    
    bitCount = _get_hightest_position(Num_L) - _get_hightest_position(Num_R);
    while (bitCount >= 0)
    {
        q = q | (1 << bitCount);                 
        Num_L = Num_L^ (Num_R << bitCount);                              //NUM_R<<bitCountb不会导致溢出，不用考虑取模
        // 仅仅是减法要用异或来处理（GF(2)上的加减运算即异或），除此之外，和
        //普通除法差不多（普通除法可能还得考虑符号的问题，但GF(2^w)域上不用）
        bitCount = _get_hightest_position(Num_L) - _get_hightest_position(Num_R);
    }
    *Remainder = Num_L;
    return q;
}

static short 
_multiplication_256(unsigned char Num_L, unsigned char Num_R)
{
    //定义变量
    int i = 0,flag;
    unsigned short Result = 0; //伽罗瓦域内乘法计算的结果
    
    for (; i < 8; i++)           //只需计算一个字节（8bit）的内容就可以了
    {
        Result ^= ((Num_L >> i) & 0x01) * Num_R;    //Relsult+=0*Num_R or 1 * Num_R
        if(Num_R & 0x80){                           //此时若再左移一位即溢出，我们要将其对本原多项式取模
            Num_R<<=1;                              //先右移，溢出了再说
            Num_R^=0x1B;      //取模。[x^8]+x^4+x^3+x+1   (x^8超过八个bit了，不用考虑它，故直接模0x1B即x^4+x^3+x+1就可以了)         
        }
        else{
            Num_R<<=1;        //不要觉得这个else好像很罗嗦，但实际上只能这样写，你不能把这一句提到if之前（当然，也可以选择用标志量的方法）
        }
    }
    
    return Result;
}

static int
_exgcd(int a,int b,int *x,int *y){
    if(!b) {
        *x=1,*y=0;
        return a;
    }
    int temp,r,gcd;
    _division((unsigned short)a,(unsigned short )b,(unsigned short *)&r);
    gcd=_exgcd(b,r,x,y);
    temp=*y;

    *y=*x^(_multiplication_256(_division((unsigned short)a,(unsigned short )b,(unsigned short *)&r),*y));
    *x=temp;
    return gcd;
}

static int
_get_mult_inverse_modulo(int a,int p){
    int x,y,temp;
    if(_exgcd(a,p,&x,&y)==1){
        _division((unsigned short)x,(unsigned short )p,(unsigned short *)&x);       
        //这一步是想取余，所以是x/p，可千万别写反了
        x=x<0?x^p:x;   
        return x;
    }
    return -1;
}

static unsigned char
_get_sbox_element(int input){
    int i=0;
    unsigned char result=0;

    for(i=0;i<8;i++){                    //观察矩阵，即可轻松找到规律
        result^=(((input>>i)&1)^((input>>(i+4)%8)&1)^((input>>(i+5)%8)&1)^
        ((input>>(i+6)%8)&1)^((input>>(i+7)%8)&1))<<i;
    }
    return result^0x63;
}

static unsigned char
_get_isbox_element(int input){
    int i=0;
    unsigned char result=0;

    for(i=0;i<8;i++){               
        result^=(((input>>(i+2)%8)&1)^((input>>(i+5)%8)&1)^((input>>(i+7)%8)&1))<<i;
    }    
    return result^0x05;
}

static void
_key_box_substitution(unsigned char (*ext_key)[44],unsigned char box[256],int coln){
    int i=0;

    for(;i<4;i++){
        ext_key[i][coln]=box[ext_key[i][coln]];
    }
}

static void
_g_func(unsigned char (*ext_key)[44],unsigned char box[256],int coln){
    const unsigned int rcon[11] = { 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };
    int i=0;

    for(;i<4;i++){
        ext_key[i][coln]=ext_key[(i+1)%4][coln-1];
    }
    _key_box_substitution( ext_key, box, coln );
    ext_key[0][coln]^=rcon[coln>>2];
}

static void
_gen_s_box(unsigned char s_box[256]){
    int i=1,mult_inverse_modulo;

    for(;i<256;i++){
        mult_inverse_modulo=_get_mult_inverse_modulo(i,0x011b);
        s_box[i]=_get_sbox_element(mult_inverse_modulo);
    }
	s_box[0]=0x63;                            
	//0没有乘法逆元，是一个特例，直接单独生成
}


static void
_gen_is_box(unsigned char is_box[256]){
    int i=0,mult_inverse_modulo;

    for(;i<256;i++){
        is_box[i]=_get_isbox_element(i);
        is_box[i]=is_box[i]?_get_mult_inverse_modulo(is_box[i],0x11b):is_box[i];     
        //0无乘法逆元，只能映射到自身
    }
}

static void
_gen_ext_key(unsigned char * key,unsigned char (*ext_key)[44],unsigned char box[256]){
    int i=0,j,k;

    for(;i<16;i++){
        ext_key[i%4][i>>2]=key[i];
    }
    
    for(i=1;i<11;i++){
        _g_func(ext_key,box,4*i);
        for(j=0;j<4;j++){
            ext_key[j][4*i]^=ext_key[j][4*(i-1)];
        }

        for(j=1;j<4;j++){
            for(k=0;k<4;k++){
                ext_key[k][4*i+j]=ext_key[k][4*(i-1)+j]^ext_key[k][4*i+j-1];
            }
        }
    }
}

static void
_string_to_array(unsigned char * s,unsigned char (*plain_array)[4]){
    int i;

    for(i=0;i<16;i++){
        plain_array[i%4][i>>2]=s[i];
    }
}

static void
_add_round_key(unsigned char (*plain_array)[4],unsigned char (*ext_key)[44],int start_col){
    int i,j;

    printf("add:%d %p %d\n",ext_key[0][36],ext_key,start_col);
    for(j=start_col,start_col+=4;j<start_col;j++){
        for(i=0;i<4;i++){
            plain_array[i][j]^=ext_key[i][j];
            // printf("%d ",ext_key[i][j]);
        }
        // printf("\n");
    }
    // printf("\n");
}

static void
_char_substitution(unsigned char *plain_array,unsigned char * box){         //转为一维数组方便处理(无所谓顺序，所以可以这样)
    int i=0;
    
    for(;i<16;i++){
        plain_array[i]=box[plain_array[i]];
    }
}

static void 
_shift_rows(unsigned int *plain_array){                  //转为一维数组方便处理（一行一行的处理，所以可以这样）。
    plain_array[1]=(plain_array[1]<<8)|(plain_array[1]>>24);
    plain_array[2]=(plain_array[2]<<16)|(plain_array[2]>>16);
    plain_array[3]=(plain_array[3]<<24)|(plain_array[3]>>8);
}

static void
_i_shift_rows(unsigned int *plain_array){                //转为一维数组方便处理（一行一行的处理，所以可以这样）。
    plain_array[1]=(plain_array[1]>>8)|(plain_array[1]<<24);
    plain_array[2]=(plain_array[2]>>16)|(plain_array[2]<<16);
    plain_array[3]=(plain_array[3]>>24)|(plain_array[3]<<8);
}

static void
_mix_colum(unsigned char (*plain_array)[4],const unsigned char (*mix_array)[4]){
    int i,j;
    unsigned char temp_array[4][4];

    memcpy(temp_array,plain_array,16);
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            plain_array[i][j]=_multiplication_256(mix_array[i][0],temp_array[0][j])^             //要用一个临时数组，可不要直接赋值。
                              _multiplication_256(mix_array[i][1],temp_array[1][j])^
                              _multiplication_256(mix_array[i][2],temp_array[2][j])^
                              _multiplication_256(mix_array[i][3],temp_array[3][j]);
        }
    }
}

int
print_box(unsigned char box[256],int flag){
    int i=0;
    
    printf("unsigned char %s={  ",flag?"s_box":"is_box");
    for(;i<256;i++){
		if(i%16==0)
            printf("\n  ");
        printf("0x%02x%c",box[i],i<255?',':' ');
    }
    printf("\n};");
    return 1;
}

void 
aes_encrypt(unsigned char * plain,unsigned char * key,unsigned char * secret,int count){
    unsigned char MixArray[4][4] ={
        0x02, 0x03, 0x01, 0x01,
        0x01, 0x02, 0x03, 0x01,
        0x01, 0x01, 0x02, 0x03,
        0x03, 0x01, 0x01, 0x02
    };
    unsigned char s_box[256],ext_key[4][44],(*plain_array)[4]=malloc(4*4);
    //狗屎，plain_array非得用malloc或者一维数组才正常！用二维数组莫名其妙的值传过去不对，不知道是不是系统的bug(还有个说法是栈内存是不保证连续的)
    //而且还不能free，怀疑整个程序有内存越界的问题，改天再好好检查一下(很有可能是操作box时越界了，因为我把box替换为常量时就报abort错误)
    int i,j,k;
    printf("plain_array:%p\n:",plain_array);
    _gen_s_box(s_box);
    _gen_ext_key(key,ext_key,s_box);
    for(k=0;k<count;k++){
        _string_to_array(plain+k*16,plain_array);
        
        _add_round_key(plain_array,ext_key,0);
        for(i=1;i<11;i++){
            _char_substitution(plain_array,s_box);
            _shift_rows(plain_array);
            if(i!=10)
                _mix_colum(plain_array,MixArray);
            printf("i:%d,%d\n",i,i*4);
            _add_round_key(plain_array,ext_key,4*i);
        }
        for(j=0;j<4;j++){
            for(i=0;i<4;i++){
                secret[j*4+i+k*16]=plain_array[i][j];
            }
        }
    }
    printf("plain_array:%p\n:",plain_array);
    // free(plain_array);
}

void
aes_decrypt(unsigned char * secret,unsigned char * key,unsigned char * plain,int count){
    unsigned char I_MixArray[4][4] ={
        0x0E, 0x0B, 0x0D, 0x09,
        0x09, 0x0E, 0x0B, 0x0D,
        0x0D, 0x09, 0x0E, 0x0B,
        0x0B, 0x0D, 0x09, 0x0E
    };
    unsigned char secret_array[4][4],s_box[256],is_box[256],(*ext_key)[44]=malloc(4*44);   
    //狗屎，ext_key非得用malloc或者一维数组才正常！用二维数组莫名其妙的值传过去不对，不知道是不是系统的bug(还有个说法是栈内存是不保证连续的)
    int i,j,k;

    _gen_is_box(is_box);
    _gen_s_box(s_box);
    _gen_ext_key(key,ext_key,s_box);
    for(k=0;k<count;k++){
        _string_to_array(secret+k*16,secret_array);
            // for(i=0;i<4*44;i++){
        //     printf("%c%d",i%4==0 && i?'\n':' ',ext_key[i%4][i/4]);
        // }
        printf("de:%d %p\n",ext_key[0][36],ext_key);
        for(i=10;i>0;i--){
            _add_round_key(secret_array,ext_key,4*i);
            if(i!=10)
                _mix_colum(secret_array,I_MixArray);
            _i_shift_rows(secret_array);
            _char_substitution(secret_array,is_box);        
        }
        _add_round_key(secret_array,ext_key,0);
        
        for(j=0;j<4;j++){
            for(i=0;i<4;i++){
                plain[j*4+i+k*16]=secret_array[i][j];
            }
        }
    }
    free(ext_key);
}




//for test


int
main(){
    unsigned char secret[16*5+1],plain[16*5+1];
    int i;
    
    aes_encrypt("hello,world,you!hello,world,you!hello,world,you!hello,world,you!hello,world,you!","zccdefkhijolabcd",secret,5);
    // for(i=0;i<16*5;i++)
    //     printf("%d ",secret[i]);
    // printf("\n");
    aes_decrypt(secret,"zccdefkhijolabcd",plain,5);
    // for(i=0;i<16;i++)
    //     printf("%d ",plain[i]);
    plain[16*5]='\0';
    printf("%s\n",plain);
 
    return 0;
}