#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * コンパイル方法:
 * gcc -O0 -fno-stack-protector lab2B.c -o lab2B
 */

char* exec_string = "/bin/sh";

void shell(char* cmd)
{
	system(cmd);
}

void print_name(char* input)
{
	char buf[15];
	strcpy(buf, input);
	printf("こんにちは、%s\n", buf);
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("使い方:\n%s 文字列\n", argv[0]);
		return EXIT_FAILURE;
	}

	print_name(argv[1]);

	return EXIT_SUCCESS;
}
