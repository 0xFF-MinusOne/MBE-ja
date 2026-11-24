/*
 *   書式文字列ラボ - 問題 C
 *   gcc -z execstack -z norelro -fno-stack-protector -o lab4C lab4C.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PASS_LEN 30

int main(int argc, char *argv[])
{
    char username[100] = {0};
    char real_pass[PASS_LEN] = {0};
    char in_pass[100] = {0};
    FILE *pass_file = NULL;
    int rsize = 0;

    /* パスワードファイルを開く */
    pass_file = fopen("/home/lab4B/.pass", "r");
    if (pass_file == NULL) {
        fprintf(stderr, "エラー: パスワードファイルを開けませんでした\n");
        exit(EXIT_FAILURE);
    }

    /* パスワードファイルの内容を読み込む */
    rsize = fread(real_pass, 1, PASS_LEN, pass_file);
    real_pass[strcspn(real_pass, "\n")] = '\0';  // \n を取り除く
    if (rsize != PASS_LEN) {
        fprintf(stderr, "エラー: パスワードファイルを読み込めませんでした\n");
        exit(EXIT_FAILURE);
    }

    /* パスワードファイルを閉じる */
    fclose(pass_file);

    puts("===== [ セキュアアクセスシステム v1.0 ] =====");
    puts("-----------------------------------------");
    puts("- このシステムにアクセスするにはログインが必要です。 -");
    puts("-----------------------------------------");

    /* ユーザー名を安全に読み込む */
    printf("--[ ユーザー名: ");
    fgets(username, 100, stdin);
    username[strcspn(username, "\n")] = '\0';    // \n を取り除く

    /* 入力されたパスワードを安全に読み込む */
    printf("--[ パスワード: ");
    fgets(in_pass, sizeof(in_pass), stdin);
    in_pass[strcspn(in_pass, "\n")] = '\0';      // \n を取り除く

    puts("-----------------------------------------");

    /* パスワードが正しければユーザーをログインさせる */
    if(!strncmp(real_pass, in_pass, PASS_LEN)){
        printf("ようこそ、%s!\n", username);
        system("/bin/sh");
    } else {
        printf(username);
        printf(" にはアクセス権がありません!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
