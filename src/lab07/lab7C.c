/* コンパイル方法: gcc -z relro -z now -fPIE -pie -fstack-protector-all -o lab7C lab7C.c */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"

#define MAX_STR 6
#define MAX_NUM 6

struct data {
    char reserved[8];
    char buffer[20];
    void (* print)(char *);
};

struct number {
    unsigned int reserved[6];               // 後で実装する
    void (* print)(unsigned int);
    unsigned int num;
};

void small_str(char * a_str)
{
    printf("しょぼい文字列だよ: %s\n", a_str);
}

void big_str(char * a_str)
{
    printf("でかい文字列だな: %s\n", a_str);
}

void small_num(unsigned int a_num)
{
    printf("1337 には程遠いな: %u\n", a_num);
}

void big_num(unsigned int a_num)
{
    printf("いかした数だぜ: %u\n", a_num);
}

void print_menu()
{
    printf("-- UAF プレイグラウンド メニュー ----------------------\n"
           "1. 文字列を作る\n"
           "2. 数値を作る\n"
           "3. 文字列を削除する\n"
           "4. 数値を削除する\n"
           "5. 文字列を表示する\n"
           "6. 数値を表示する\n"
           "7. 終了する\n"
           "---------------------------------------------\n"
           "選択してください: ");
}

/* バグだらけ... でもメモリ破壊はなし! */
int main(int argc, char * argv[])
{
    struct data * strings[MAX_STR] = {0};
    struct number * numbers[MAX_NUM] = {0};
    struct data * tempstr = NULL;
    struct number * tempnum = NULL;

    int strcnt = 0;
    int numcnt = 0;
    unsigned int choice = 0;
    unsigned int index = 0;

    while(1)
    {
        print_menu();

        /* メニューの選択肢を取得する */
        if((choice = get_unum()) == EOF)
            break;

        /* 文字列を作る */
        if(choice == 1)
        {
            if(strcnt < MAX_STR)
            {
                tempstr = malloc(sizeof(struct data));

                /* 今回はメモリ破壊なし */
                printf("保存する文字列を入力してください: ");
                fgets(tempstr->buffer, 20, stdin);
                tempstr->buffer[strcspn(tempstr->buffer, "\n")] = 0;

                /* 表示用の関数を選ぶ */
                tempstr->print = strlen(tempstr->buffer) > 10 ? big_str : small_str;

                /* 文字列をマスターリストに保存する */
                strings[++strcnt] = tempstr;
                printf("新しい文字列を作成しました!\n");
            }
            else
                printf("別の文字列を作る前に、文字列を削除してください!\n");
        }

        /* 数値を作る */
        else if(choice == 2)
        {
            if(numcnt < MAX_NUM)
            {
                tempnum = malloc(sizeof(struct number));

                printf("保存する数値を入力してください: ");
                tempnum->num = get_unum();

                /* 表示用の関数を選ぶ */
                tempnum->print = tempnum->num > 0x31337 ? big_num : small_num;

                /* 数値をマスターリストに保存する */
                numbers[++numcnt] = tempnum;
                printf("新しい数値を作成しました!\n");
            }
            else
                printf("別の数値を作る前に、数値を削除してください!\n");
        }

        /* 文字列を削除する */
        else if(choice == 3)
        {
            if(strcnt && strings[strcnt])
            {
                free(strings[strcnt--]);
                printf("直近の文字列を削除しました!\n");
            }
            else
                printf("削除できる文字列が残っていません!\n");
        }

        /* 数値を削除する */
        else if(choice == 4)
        {
            if(numcnt && numbers[numcnt])
            {
                free(numbers[numcnt--]);
                printf("直近の数値を削除しました!\n");
            }
            else
                printf("削除できる数値が残っていません!\n");
        }

        /* 文字列を表示する */
        else if(choice == 5)
        {
            printf("表示する文字列のインデックス: ");
            index = get_unum();

            if(index < MAX_STR && strings[index])
                strings[index]->print(strings[index]->buffer);
            else
                printf("表示できる文字列がありません!\n");
        }

        /* 数値を表示する */
        else if(choice == 6)
        {
            printf("表示する数値のインデックス: ");
            index = get_unum();

            if(index < MAX_NUM && numbers[index])
                numbers[index]->print(numbers[index]->num);
            else
                printf("表示できる数値がありません!\n");
        }

        /* 終了 */
        else if(choice == 7)
            break;

        /* 上記以外 */
        else
            printf("選択肢が不正です!\n");

        index = 0;
        choice = 0;
        printf("\n");
    }

    printf("また明日!\n");
    return EXIT_SUCCESS;
}
