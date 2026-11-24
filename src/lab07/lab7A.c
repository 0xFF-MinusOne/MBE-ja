/* コンパイル方法: gcc -static -z relro -z now -fstack-protector-all -o lab7A lab7A.c */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

ENABLE_TIMEOUT(60)

#define MAX_MSG    10
#define MAX_BLOCKS 32
#define BLOCK_SIZE 4

struct msg {
    void (* print_msg)(struct msg *);
    unsigned int xor_pad[MAX_BLOCKS];
    unsigned int message[MAX_BLOCKS];
    unsigned int msg_len;
};

struct msg * messages[MAX_MSG];

/* ワンタイムパッドを適用する */
void encdec_message(unsigned int * message, unsigned int * xor_pad)
{
    int i = 0;
    for(i = 0; i < MAX_BLOCKS; i++)
        message[i] ^= xor_pad[i];
}

/* 指定されたメッセージの情報を表示する */
void print_message(struct msg * to_print)
{
    unsigned int i = 0;
    char * xor_pad;
    char * message;

    xor_pad = (char *)&to_print->xor_pad;
    message = (char *)&to_print->message;

    /* メッセージの XOR パッドを表示する */
    printf("\nXOR パッド: \n"
           "-----------------------------------------\n");

    for(i = 0; i < BLOCK_SIZE*MAX_BLOCKS; i++)
    {
        printf("%02x", xor_pad[i] & 0xFF);
        if(i % 32 == 31)
            puts("");
    }

    /* 暗号化されたメッセージを表示する */
    printf("\n暗号化されたメッセージ: \n"
           "-----------------------------------------\n");

    for(i = 0; i < BLOCK_SIZE*MAX_BLOCKS; i++)
    {
        printf("%02x", message[i] & 0xFF);
        if(i % 32 == 31)
            puts("");
    }

    puts("");
}

/* メッセージを作成する */
int create_message()
{
    int i, j;
    struct msg * new_msg = NULL;

    /* 空いているメッセージスロットを探す */
    for(i = 0; i < MAX_MSG; i++)
        if(messages[i] == NULL)
            break;

    /* 本当に空きスロットが見つかったことを確認する */
    if(messages[i])
    {
        printf("-メッセージスロットが残っていません!\n");
        return 1;
    }

    printf("-メッセージスロット #%u を使います\n", i);

    /* 新しいメッセージを初期化する */
    new_msg = malloc(sizeof(struct msg));
    memset(new_msg, 0, sizeof(struct msg));
    new_msg->print_msg = &print_message;

    for(j = 0; j < MAX_BLOCKS; j++)
        new_msg->xor_pad[j] = rand();

    /* ユーザーが暗号化しようとしているデータの長さを取得する */
    printf("-データの長さを入力してください: ");

    new_msg->msg_len = get_unum();

    if(new_msg->msg_len == 0)
    {
        printf("-メッセージの長さは 0 より大きくなければなりません!\n");
        free(new_msg);
        return 1;
    }

    /* メッセージの長さが XOR パッドより大きくないことを確認する */
    if((new_msg->msg_len / BLOCK_SIZE) > MAX_BLOCKS)
        new_msg->msg_len = BLOCK_SIZE * MAX_BLOCKS;

    /* XOR パッドで暗号化するメッセージを読み込む */
    printf("-暗号化するデータを入力してください: ");
    read(0, &new_msg->message, new_msg->msg_len);

    /* メッセージを暗号化する */
    encdec_message(new_msg->message, new_msg->xor_pad);

    /* 新しいメッセージをグローバルなリストに保存する */
    messages[i] = new_msg;

    return 0;
}

int edit_message()
{
    char numbuf[32];
    unsigned int i = 0;

    /* 対象のメッセージのインデックスを取得する */
    printf("-編集するメッセージのインデックスを入力してください: ");
    fgets(numbuf, sizeof(numbuf), stdin);
    i = strtoul(numbuf, NULL, 10);

    if(i >= MAX_MSG || messages[i] == NULL)
    {
        printf("-メッセージのインデックスが不正です!\n");
        return 1;
    }

    printf("-暗号化する新しいメッセージを入力してください: ");

    /* 古いメッセージを消去して、新しいメッセージを読み込む */
    memset(&messages[i]->message, 0, BLOCK_SIZE * MAX_BLOCKS);
    read(0, &messages[i]->message, messages[i]->msg_len);

    /* メッセージを暗号化する */
    encdec_message(messages[i]->message, messages[i]->xor_pad);

    return 0;
}

/* 安全なメッセージを解放する */
int destroy_message()
{
    char numbuf[32];
    unsigned int i = 0;

    /* 対象のメッセージのインデックスを取得する */
    printf("-破棄するメッセージのインデックスを入力してください: ");
    fgets(numbuf, sizeof(numbuf), stdin);
    i = strtoul(numbuf, NULL, 10);

    if(i >= MAX_MSG || messages[i] == NULL)
    {
        printf("-メッセージのインデックスが不正です!\n");
        return 1;
    }

    /* メッセージを破棄する */
    memset(messages[i], 0, sizeof(struct msg));
    free(messages[i]);
    messages[i] = NULL;

    return 0;
}

/* 選択したインデックスのメッセージを表示する */
int print_index()
{
    char numbuf[32];
    unsigned int i = 0;

    /* 表示するメッセージのインデックスを取得する */
    printf("-表示するメッセージのインデックスを入力してください: ");
    fgets(numbuf, sizeof(numbuf), stdin);
    i = strtoul(numbuf, NULL, 10);

    if(i >= MAX_MSG || messages[i] == NULL)
    {
        printf("-メッセージのインデックスが不正です!\n");
        return 1;
    }

    /* 対象のメッセージを表示する */
    messages[i]->print_msg(messages[i]);

    return 0;
}

/* 脆弱性はこの中にあります */
void print_menu()
{
    printf("+---------------------------------------+\n"
           "|       Doom の OTP サービス v1.0       |\n"
           "+---------------------------------------+\n"
           "|---------- サービスメニュー -----------|\n"
           "|---------------------------------------|\n"
           "| 1. 安全なメッセージを作成する         |\n"
           "| 2. 安全なメッセージを編集する         |\n"
           "| 3. 安全なメッセージを破棄する         |\n"
           "| 4. メッセージの詳細を表示する         |\n"
           "| 5. 終了する                           |\n"
           "+---------------------------------------+\n");
}

int main()
{
    int choice = 0;
    srand(time(NULL));
    disable_buffering(stdout);

    while(1)
    {
        print_menu();

        /* メニューの選択肢を取得する */
        printf("選択してください: ");
        choice = get_unum();

        printf("-----------------------------------------\n");

        /* メニューの選択を処理する */
        if(choice == 1)
        {
            if(create_message())
                printf("-メッセージの作成に失敗しました!\n");
            else
                printf("-メッセージを作成しました!\n");
        }
        else if(choice == 2)
        {
            if(edit_message())
                printf("-メッセージの編集に失敗しました!\n");
            else
                printf("-メッセージを変更しました!\n");
        }
        else if(choice == 3)
        {
            if(destroy_message())
                printf("-メッセージの破棄に失敗しました!\n");
            else
                printf("-メッセージを破棄しました!\n");
        }
        else if(choice == 4)
        {
            if(print_index())
                printf("-メッセージの表示に失敗しました!\n");
        }
        else if(choice == 5)
        {
            break;  // 終了
        }
        else
            printf("-選択肢が不正です!\n");

        choice = 0;
        puts("");
    }

    printf("また明日!\n");
    return EXIT_SUCCESS;
}
