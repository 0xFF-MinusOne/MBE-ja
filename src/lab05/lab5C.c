#include <stdlib.h>
#include <stdio.h>

/* gcc -fno-stack-protector -o lab5C lab5C.c */

char global_str[128];

/* 文字列を読み込み、グローバル変数にコピーする */
void copytoglobal()
{
    char buffer[128] = {0};
    gets(buffer);
    memcpy(global_str, buffer, 128);
}

int main()
{
    char buffer[128] = {0};

    printf("あなたのために libc を含めておきました...\n"\
           "system() まで ROP できますか?\n");

    copytoglobal();

    return EXIT_SUCCESS;
}
