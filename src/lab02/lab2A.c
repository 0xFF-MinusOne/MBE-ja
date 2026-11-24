#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * コンパイル方法:
 * gcc -O0 -fno-stack-protector lab2A.c -o lab2A
 */

void shell()
{
	printf("やりましたね\n");
	system("/bin/sh");
}

void concatenate_first_chars()
{
	struct {
		char word_buf[12];
		int i;
		char* cat_pointer;
		char cat_buf[10];
	} locals;
	locals.cat_pointer = locals.cat_buf;

	printf("10 個の単語を入力してください:\n");
	for(locals.i=0; locals.i!=10; locals.i++)
	{
		// 標準入力から読み込む
		if(fgets(locals.word_buf, 0x10, stdin) == 0 || locals.word_buf[0] == '\n')
		{
			printf("単語の読み込みに失敗しました\n");
			return;
		}
		// 単語の先頭の 1 文字を、連結用バッファの次の位置にコピーする
		*locals.cat_pointer = *locals.word_buf;
		locals.cat_pointer++;
	}

	// たとえ何かがおかしくなっても、ここに null バイトがあるので、
	//   バッファオーバーフローは防げる
	locals.cat_buf[10] = '\0';
	printf("10 個の単語の先頭文字を連結したものです:\n\
%s\n", locals.cat_buf);
}

int main(int argc, char** argv)
{
	if(argc != 1)
	{
		printf("使い方:\n%s\n", argv[0]);
		return EXIT_FAILURE;
	}

	concatenate_first_chars();

	printf("認証されていません\n");
	return EXIT_SUCCESS;
}
