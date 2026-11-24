/*
ASLR 有効下でのエクスプロイト
ラボ A

gcc -fpie -pie -fno-stack-protector -o lab6A ./lab6A.c

Patrick Biernat
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

struct uinfo {
    char name[32];
    char desc[128];
    unsigned int sfunc;
}user;


struct item {
    char name[32];
    char price[10];
}aitem;

struct item ulisting;

void write_wrap(char ** buf) {
    write(1, *buf, 8);
}

void make_note() {
    char note[40];
    printf("出品物についてのメモを書いてください...: ");
    gets(note);
}

void print_listing() {
    printf(
    "作成した出品物はこちらです: \n");
    if(*ulisting.name == '\x00') {
        return;
    }
    printf("商品: %s\n", ulisting.name);
    printf("価格: %s\n",ulisting.price);
}

void make_listing() {
    printf("商品名を入力してください: ");
    fgets(ulisting.name, 31, stdin);
    printf("商品の価格を入力してください: ");
    fgets(ulisting.price, 9, stdin);
}

void setup_account(struct uinfo * user) {
    char temp[128];
    memset(temp, 0, 128);
    printf("名前を入力してください: ");
    read(0, user->name, sizeof(user->name));
    printf("説明を入力してください: ");
    read(0, temp, sizeof(user->desc));
    strncpy(user->desc, user->name,32);
    strcat(user->desc, " is a ");

    memcpy(user->desc + strlen(user->desc), temp, strlen(temp));
}

void print_name(struct uinfo * info) {
    printf("ユーザー名: %s\n", info->name);
}

int main(int argc, char ** argv) {
    disable_buffering(stdout);
    struct uinfo  merchant;
    char choice[4];

    printf(
    ".-------------------------------------------------. \n" \
    "|  l337-Bay へようこそ                          + | \n"
    "|-------------------------------------------------| \n"
    "|1: アカウントを設定する                          | \n"
    "|2: 出品物を作成する                              | \n"
    "|3: 情報を表示する                                | \n"
    "|4: 終了する                                      | \n"
    "|-------------------------------------------------| \n" );

    // ユーザー情報を初期化する
    memset(merchant.name, 0, 32);
    memset(merchant.desc, 0 , 64);
    merchant.sfunc = (unsigned int)print_listing;

    // 出品物を初期化する
    memset(ulisting.name, 0, 32);
    memset(ulisting.price, 0, 10);

    while(1) {
        memset(choice, 0, 4);
        printf("選択してください: ");

        if (fgets(choice, 2, stdin) == 0) {
            break;
        }
        getchar(); // 改行を読み捨てる

        if (!strncmp(choice, "1",1)) {
            setup_account(&merchant);
        }
        if (!strncmp(choice, "2",1)) {
            make_listing();
        }
        if (!strncmp(choice, "3",1)) { // まるで C にクラスがあるみたいだ!
            ( (void (*) (struct uinfo *) ) merchant.sfunc) (&merchant);
        }
        if (!strncmp(choice, "4",1)) {
            return EXIT_SUCCESS;
        }

    }


    return EXIT_SUCCESS;
}
