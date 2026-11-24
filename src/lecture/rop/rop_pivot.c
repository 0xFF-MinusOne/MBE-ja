#include <stdio.h>

/* gcc -static -o rop_pivot rop_pivot.c */

int main(int argc, char * argv[])
{
    char buffer[128] = {0};
    int overwrite[1] = {0};

    if(argc < 3)
    {
        printf("引数が 2 つ必要です\n");
    	printf("使い方: %s [上書きするインデックス] [上書きするアドレス]\n", argv[0]);
        return 1;
    }

    /* ピボットの状況を作る */
    printf("0x%08x を 0x%08x で上書きします\n", \
	overwrite[strtoul(argv[1], NULL, 10)], strtoul(argv[2], NULL, 10));
    overwrite[strtoul(argv[1], NULL, 10)] = strtoul(argv[2], NULL, 10);

    /* ROP チェーンを読み込む */
    printf("ROP チェーンを入力してください: ");
    fgets(buffer, 128, stdin);

    /* プログラムは常に 0 を返す */
    return 0;
}

