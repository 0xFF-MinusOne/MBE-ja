#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * コンパイル方法:
 * gcc -O0 -fno-stack-protector lab2C.c -o lab2C
 */

void shell()
{
	printf("やりましたね。\n");
	system("/bin/sh");
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("使い方:\n%s 文字列\n", argv[0]);
		return EXIT_FAILURE;
	}

	int set_me = 0;
	char buf[15];
	strcpy(buf, argv[1]);

	if(set_me == 0xdeadbeef)
	{
		shell();
	}
	else
	{
		printf("認証されていません。\nset_me は %d でした\n", set_me);
	}

	return EXIT_SUCCESS;
}
