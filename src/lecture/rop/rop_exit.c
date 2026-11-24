#include <stdio.h>

int main()
{
    char buffer[128] = {0};

    /* 色々なものを壊す */
    printf("ROP チェーンを入力してください: ");
    gets(buffer);
    
    /* プログラムは常に 0 を返す */
    return 0;
}
