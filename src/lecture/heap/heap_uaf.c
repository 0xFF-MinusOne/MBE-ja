/* doom は gcc の保護機能に手を抜かない。すべて有効だ! */
/* コンパイル方法: gcc -z relro -z now -fPIE -pie -fstack-protector-all -o heap_uaf heap_uaf.c */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

struct toystr {
    void (* message)(char *);
    char buffer[20];
};

struct person {
    int favorite_num;
    int age;
    char name[16];
};

void print_cool(char * who)
{
    printf("%s はかっこいい!\n", who);
}

void print_meh(char * who)
{
    printf("%s はいまいち...\n", who);
}

void secret_shell()
{
    execve("/bin/sh", NULL, NULL);
}

void print_menu()
{
    printf("-- メニュー ----------------------\n"
           "1. coolguy を作る\n"
           "2. a_person を作る\n"
           "3. coolguy を削除する\n"
           "4. a_person を削除する\n"
           "5. coolguy のメッセージを表示する\n"
           "6. person の情報を表示する\n"
           "7. 終了する\n"
           "選択してください: ");
}

/* バグだらけ... でもメモリ破壊はなし! */
int main(int argc, char * argv[])
{
    struct toystr * coolguy = NULL;
    struct person * a_person = NULL;
    int choice = 0;

    while(1)
    {
        print_menu();
        
        /* メニューの選択肢を取得する */
        scanf("%u", &choice);
        getc(stdin);

        /* メニューの処理を実行する */
        if(choice == 1)
        {
            coolguy = malloc(sizeof(struct toystr));
            printf("新しい coolguy は 0x%08x にあります\n", (unsigned int)coolguy);
            
            /* 今回はメモリ破壊なし */
            printf("coolguy の名前を入力してください: ");
            fgets(coolguy->buffer, 20, stdin);
            coolguy->buffer[strcspn(coolguy->buffer, "\n")] = 0;
            coolguy->message = &print_cool;

            /* やったー */
            printf("新しい coolguy を作成しました!\n");
        }
        else if(choice == 2)
        {
            a_person = malloc(sizeof(struct person));
            printf("新しい person は 0x%08x にあります\n", (unsigned int)a_person);

            /* a_person を初期化する */
            printf("a_person の名前を入力してください: ");
            fgets(a_person->name, 16, stdin);
            a_person->name[strcspn(a_person->name, "\n")] = 0;
            
            printf("a_person の好きな数を入力してください: ");
            scanf("%u", &a_person->favorite_num);
            getc(stdin);

            printf("a_person の年齢を入力してください: ");
            scanf("%u", &a_person->age);
            getc(stdin);
            if(a_person->age > 110 || a_person->age < 0)
                printf("うわ、年齢がかなりぶっ飛んでるね\n");

            /* ここまでで完了 */
            printf("新しい person を作成しました!\n");
        }
        else if(choice == 3)
        {
            if(coolguy)
            {
                free(coolguy);
                printf("coolguy を削除しました!\n");
            }
            else
                printf("解放できる coolguy がありません!\n");
        }
        else if(choice == 4)
        {
            if(a_person)
            {
                free(a_person);
                printf("person を削除しました!\n");
            }
            else
                printf("解放できる person がありません!\n");
        }
        else if(choice == 5)
        {
            if(coolguy)
                coolguy->message(coolguy->buffer);
            else
                printf("かっこいいメッセージを表示できる coolguy がありません!\n");
        }
        else if(choice == 6)
        {
            if(a_person)
            {
                printf("person の名前: %s\n", a_person->name);
                printf("person の年齢: %d\n", a_person->age);
                printf("person の好きな数: %u\n", a_person->favorite_num);
            }
            else
                printf("表示できる person がありません!\n");
        }
        else if(choice == 7)
            break;
        else
            printf("選択肢が不正です!\n");
      
        choice = 0;
    }

    printf("また明日!\n");
    return 0;
}
