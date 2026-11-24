/*
ASLR 有効下でのエクスプロイト
ラボ C

 gcc -pie -fPIE -fno-stack-protector -o lab6C lab6C.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct savestate {
    char tweet[140];
    char username[40];
    int msglen;
} save;

void set_tweet(struct savestate *save );
void set_username(struct savestate * save);

/* デバッグ用の機能。本番では使われない */
void secret_backdoor()
{
    char cmd[128];

    /* コマンドを読み込んで実行する */
    fgets(cmd, 128, stdin);
    system(cmd);

    return;
}

void handle_tweet()
{
    struct savestate save;

    /* 保存状態を正常な値で初期化する */
    memset(save.username, 0, 40);
    save.msglen = 140;

    /* ユーザーからユーザー名とツイートを読み込む */
    set_username(&save);
    set_tweet(&save);

    printf(">: ツイートを送信しました!\n");
    return;
}

void set_tweet(struct savestate *save )
{
    char readbuf[1024];
    memset(readbuf, 0, 1024);

    printf(">: @Unix-Dude にツイートする\n");
    printf(">>: ");

    /* ユーザーからツイートを読み込み、構造体に安全にコピーする */
    fgets(readbuf, 1024, stdin);
    strncpy(save->tweet, readbuf, save->msglen);

    return;
}

void set_username(struct savestate * save)
{
    int i;
    char readbuf[128];
    memset(readbuf, 0, 128);

    printf(">: ユーザー名を入力してください\n");
    printf(">>: ");

    /* ユーザー名を読み込み、保存状態にコピーする */
    fgets(readbuf, 128, stdin);
    for(i = 0; i <= 40 && readbuf[i]; i++)
        save->username[i] = readbuf[i];

    printf(">: ようこそ、%s", save->username);
    return;
}

int main(int argc, char * argv[])
{

    printf(
    "--------------------------------------------\n" \
    "|   ~l33t-tw33ts へようこそ ~    v.0.13.37 |\n" \
    "--------------------------------------------\n");

    /* ツイートをいくつか作る */
    handle_tweet();

    return EXIT_SUCCESS;
}
