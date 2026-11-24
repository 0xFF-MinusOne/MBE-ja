#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

#define STORAGE_SIZE 100

/* gcc --static -o lab5A lab5A.c */

/* ユーザーから数値を受け取って保存する */
int store_number(unsigned int * data)
{
    unsigned int input = 0;
    int index = 0;

    /* 保存する数値を取得する */
    printf(" 数値: ");
    input = get_unum();

    /* 保存先のインデックスを取得する */
    printf(" インデックス: ");
    index = (int)get_unum();

    /* そのスロットが予約済みでないことを確認する */
    if(index % 3 == 0 || index > STORAGE_SIZE || (input >> 24) == 0xb7)
    {
        printf(" *** エラー! ***\n");
        printf("   このインデックスは doom 用に予約されています!\n");
        printf(" *** エラー! ***\n");

        return 1;
    }

    /* 数値をデータ保管領域に保存する */
    data[index] = input;

    return 0;
}

/* 指定された保管領域のインデックスの内容を返す */
int read_number(unsigned int * data)
{
    int index = 0;

    /* 読み込むインデックスを取得する */
    printf(" インデックス: ");
    index = (int)get_unum();

    printf(" Number at data[%d] is %u\n", index, data[index]);

    return 0;
}

int main(int argc, char * argv[], char * envp[])
{
    int res = 0;
    char cmd[20] = {0};
    unsigned int data[STORAGE_SIZE] = {0};

    /* doom は環境変数が嫌いです */
    clear_argv(argv);
    clear_envp(envp);

    printf("----------------------------------------------------\n"\
           "  doom のしょぼい数値保管サービスへようこそ!\n"\
           "          バージョン 2.0 - セキュリティ強化版!\n"\
           "----------------------------------------------------\n"\
           " コマンド:\n"\
           "    store - データ保管領域に数値を保存します\n"\
           "    read  - データ保管領域から数値を読み出します\n"\
           "    quit  - プログラムを終了します\n"\
           "----------------------------------------------------\n"\
           "   doom は自分用に保管領域を少し確保しています :>\n"\
           "----------------------------------------------------\n"\
           "\n");


    /* コマンド処理ループ */
    while(1)
    {
        /* このループの繰り返しの準備 */
        printf("コマンドを入力: ");
        res = 1;

        /* ユーザーの入力を読み込み、改行を取り除く */
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd)-1] = '\0';

        /* ユーザーが指定したコマンドを選ぶ */
        if(!strncmp(cmd, "store", 5))
            res = store_number(data);
        else if(!strncmp(cmd, "read", 4))
            res = read_number(data);
        else if(!strncmp(cmd, "quit", 4))
            break;

        /* コマンドの結果を表示する */
        if(res)
            printf(" %s コマンドに失敗しました\n", cmd);
        else
            printf(" %s コマンドは正常に完了しました\n", cmd);

        memset(cmd, 0, sizeof(cmd));
    }

    return EXIT_SUCCESS;
}
