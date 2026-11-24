/*
ASLR 有効下でのエクスプロイト
講義: 例 1
Modern Binary Exploitation

コンパイル方法:
gcc -pie -fPIE -fno-stack-protector -o aslr_leak1 aslr_leak1.c

-Patrick Biernat
*/
#include <stdio.h>
#include <stdlib.h>

void i_am_rly_leet() 
{
    printf("見つけましたね!\nよくできました!\n");
    return;
}

int main(int argc, char** argv)
{
    char my_string[16];
    printf("Win 関数 @ %p\n",i_am_rly_leet);
    fflush(stdout);
    fgets(my_string,40,stdin);
    
  
    return 0;
}
