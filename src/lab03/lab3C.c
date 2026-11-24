#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* gcc -z execstack -fno-stack-protector -o lab3C lab3C.c */

char a_user_name[100];

int verify_user_name()
{
    puts("ユーザー名を検証中....\n");
    return strncmp(a_user_name, "rpisec", 6);
}

int verify_user_pass(char *a_user_pass)
{
    return strncmp(a_user_pass, "admin", 5);
}

int main()
{
    char a_user_pass[64] = {0};
    int x = 0;

    /* ユーザー名の入力を促す - 100 バイト読み込む */
    printf("********* 管理者ログイン *********\n");
    printf("ユーザー名を入力: ");
    fgets(a_user_name, 0x100, stdin);

    /* 入力されたユーザー名を検証する */
    x = verify_user_name();
    if (x != 0){
        puts("違います、ユーザー名が正しくありません...\n");
        return EXIT_FAILURE;
    }

    /* 管理者パスワードの入力を促す - 64 バイト読み込む */
    printf("パスワードを入力: \n");
    fgets(a_user_pass, 0x64, stdin);

    /* 入力されたパスワードを検証する */
    x = verify_user_pass(a_user_pass);
    if (x == 0 || x != 0){
        puts("違います、パスワードが正しくありません...\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
