/* コンパイル方法: gcc -o sizes sizes.c */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{

    unsigned int lengths[] = {32, 4, 20, 0, 64, 32, 32, 32, 32, 32};
    unsigned int * ptr[10];
    int i;
   
    /* ヒープ上に任意のチャンクを作る */
    for(i = 0; i < 10; i++)
        ptr[i] = malloc(lengths[i]);
   
    /* チャンク間の距離、つまりチャンクのサイズを表示する */
    for(i = 0; i < 9; i++)
        printf("malloc(%2d) は 0x%08x にあり、次のポインタまで %3d バイトです\n", 
                lengths[i],
                (unsigned int)ptr[i],
                (ptr[i+1]-ptr[i])*sizeof(unsigned int));
   

   return 0;
}
