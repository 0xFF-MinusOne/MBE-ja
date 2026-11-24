/*
 *   書式文字列ラボ - 問題 B
 *   gcc -z execstack -z norelro -fno-stack-protector -o lab4B lab4B.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int i = 0;
    char buf[100];

    /* ユーザーの入力を安全に読み込む */
    fgets(buf, 100, stdin);

    /* 文字列を小文字に変換する */
    for (i = 0; i < strlen(buf); i++)
        if (buf[i] >= 'A' && buf[i] <= 'Z')
            buf[i] = buf[i] ^ 0x20;

    /* 新しくできた小文字の文字列をきれいに表示する */
    printf(buf);

    exit(EXIT_SUCCESS);
    return EXIT_FAILURE;
}
