#include <stdlib.h>
#include <stdio.h>

/* gcc -fno-stack-protector --static -o lab5B lab5B.c */

int main()
{

    char buffer[128] = {0};

    printf("ここに ROP チェーンを入力してください:\n");
    gets(buffer);

    return EXIT_SUCCESS;
}
