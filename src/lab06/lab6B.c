/* コンパイル方法: gcc -z relro -z now -pie -fPIE -fno-stack-protector -o lab6B lab6B.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

ENABLE_TIMEOUT(300)

/* ユーザーをログインさせる */
int login()
{
    printf("ようこそ、FALK さん\n");

    /* あなたの勝ちです */
    system("/bin/sh");
    return 0;
}

/* doom の超極秘のパスワード変換方式 */
void hash_pass(char * password, char * username)
{
    int i = 0;

    /* ユーザー名の文字でパスワードをハッシュ化する */
    while(password[i] && username[i])
    {
        password[i] ^= username[i];
        i++;
    }

    /* パスワードの残りを埋め文字でハッシュ化する */
    while(password[i])
    {
        password[i] ^= 0x44;
        i++;
    }

    return;
}

/* doom の超安全なパスワード読み込み関数 */
int load_pass(char ** password)
{
    FILE * fd = 0;
    int fail = -1;
    int psize = 0;

    /* パスワードファイルを開く */
    fd = fopen("/home/lab6A/.pass", "r");
    if(fd == NULL)
    {
        printf("秘密のパスを開けませんでした!\n");
        return fail;
    }

    /* パスワードのサイズを取得する */
    if(fseek(fd, 0, SEEK_END))
    {
        printf("パスの末尾へのシークに失敗しました!\n");
        return fail;
    }

    psize = ftell(fd);

    if(psize == 0 || psize == -1)
    {
        printf("パスのサイズを取得できませんでした!\n");
        return fail;
    }

    /* ストリームをリセットする */
    if(fseek(fd, 0, SEEK_SET))
    {
        printf("パスの先頭へのシークに失敗しました!\n");
        return fail;
    }

    /* パスワード用のバッファを確保する */
    *password = (char *)malloc(psize);
    if(password == NULL)
    {
        printf("パス用の malloc に失敗しました!\n");
        return fail;
    }

    /* パスワード全体を読み込めたことを確認する */
    if(fread(*password, sizeof(char), psize, fd) != psize)
    {
        printf("秘密のパスを読み込めませんでした!\n");
        free(*password);
        return fail;
    }

    fclose(fd);

    /* パスワードの読み込みに成功した */
    return psize;
}

int login_prompt(int pwsize, char * secretpw)
{
    char password[32];
    char username[32];
    char readbuff[128];
    int attempts = -3;
    int result = -1;

    /* ログインプロンプトのループ */
    while(attempts++)
    {
        /* データの再利用を防ぐため、バッファをクリアする */
        memset(password, 0, sizeof(password));
        memset(username, 0, sizeof(username));
        memset(readbuff, 0, sizeof(readbuff));

        /* ユーザー名を安全に読み込む */
        printf("ユーザー名を入力してください: ");
        fgets(readbuff, sizeof(readbuff), stdin);

        /* 安全な strncpy で、読み込みバッファからユーザー名をコピーする */
        strncpy(username, readbuff, sizeof(username));

        /* パスワードを安全に読み込む */
        printf("パスワードを入力してください: ");
        fgets(readbuff, sizeof(readbuff), stdin);

        /* 安全な strncpy で、読み込みバッファからパスワードをコピーする */
        strncpy(password, readbuff, sizeof(password));

        /* 今回の試行で入力されたパスワードをハッシュ化する */
        hash_pass(password, username);

        /* パスワードが正しいか確認する */
        if(pwsize > 16 && memcmp(password, secretpw, pwsize) == 0)
        {
            login();
            result = 0;
            break;
        }

        printf("ユーザー %s の認証に失敗しました\n", username);
    }

    return result;
}

int main(int argc, char* argv[])
{
    int pwsize;
    char * secretpw;

    disable_buffering(stdout);

    /* 秘密のパスを読み込む */
    pwsize = load_pass(&secretpw);
    pwsize = pwsize > 32 ? 32 : pwsize;

    /* パスワードの読み込みに失敗した */
    if(pwsize == 0 || pwsize == -1)
        return EXIT_FAILURE;

    /* 比較対象のパスワードをハッシュ化する */
    hash_pass(secretpw, "lab6A");
    printf("----------- FALK OS ログイン -----------\n");
    fflush(stdout);

    /* 認証ループ */
    if(login_prompt(pwsize, secretpw))
    {

        /* ハッカーを追い払うための、とても真剣な警告を表示する */
        printf("+-------------------------------------------------------+\n"\
               "|WARNINGWARNINGWARNINGWARNINGWARNINGWARNINGWARNINGWARNIN|\n"\
               "|GWARNINGWARNI - TOO MANY LOGIN ATTEMPTS - NGWARNINGWARN|\n"\
               "|INGWARNINGWARNINGWARNINGWARNINGWARNINGWARNINGWARNINGWAR|\n"\
               "+-------------------------------------------------------+\n"\
               "|        このセッションを記録しました。これから         |\n"\
               "|   適切な CCDC CTF チームに送り、分析してもらいます    |\n"\
               "|             -----------------------------             |\n"\
               "|        派遣された CCDC サイバーチームは、その         |\n"\
               "|     卓越した IT とネットワークの技術を駆使して、      |\n"\
               "|   あなたを追跡し、迅速なアメリカ式の裁きを下します    |\n"\
               "+-------------------------------------------------------+\n");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
