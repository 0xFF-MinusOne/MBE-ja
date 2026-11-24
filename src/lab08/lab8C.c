/*
 * gcc -z relro -z now -fPIE -pie -fstack-protector-all -o lab8C lab8C.c
 */

#include<errno.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>

struct fileComp {
	char fileContents1[255];
	char fileContents2[255];
	int cmp;
};

char* readfd(int fd)
{
	// ファイルの長さを調べる
	int size = lseek(fd, 0, SEEK_END);
	if(size >= 255)
	{
		printf("ファイルが大きすぎます。\n");
		exit(EXIT_FAILURE);
	}
	// fd をファイルの先頭に戻す
	lseek(fd, 0, SEEK_SET);
	// ファイルの内容と null バイトの分の領域を確保する
	char* fileContents = malloc((size+1) & 0xff);
	if(!fileContents)
	{
		printf("ファイルの内容用の領域を確保できませんでした\n");
		exit(EXIT_FAILURE);
	}
	// ファイルの内容をバッファに読み込む
	int numRead = read(fd, fileContents, size & 0xff);
	return fileContents;
}

int getfd(char* arg)
{
	if(arg[0] != '-' || arg[1] != 'f' || arg[3] != '=')
	{
		printf("引数 \"%s\" の書式が不正です\n", arg);
		return -1;
	}

	int fd;
	if(arg[2] == 'n')
	{
		// O_NOFOLLOW は、シンボリックリンクをたどらないという意味です。ごめんなさい。
		fd = open(arg+4, O_NOFOLLOW | O_RDONLY);
		if(fd == -1)
		{
			printf("ファイルを開けませんでした\n");
			return -1;
		}
	}
	else if(arg[2] == 'd')
	{
		errno = 0;
		fd = atoi(arg+4);
	}
	else
	{
		printf("引数 \"%s\" の書式が不正です\n", arg);
		return -1;
	}

	return fd;
}

struct fileComp* comparefds(int fd1, int fd2)
{
	struct fileComp* fc = malloc(sizeof(struct fileComp));
	if(!fc)
	{
		printf("ファイルの内容用の領域を確保できませんでした\n");
		exit(EXIT_FAILURE);
	}

	strcpy(fc->fileContents1, readfd(fd1));
	strcpy(fc->fileContents2, readfd(fd2));
	fc->cmp = strcmp(fc->fileContents1, fc->fileContents2);
	return fc;
}

char* securityCheck(char* arg, char* s)
{
	if(strstr(arg, ".pass"))
		return "<<<セキュリティ上の理由により、ファイル名はブロックされました>>>";
	return s;
}

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		printf("こんにちは。このプログラムは、2 つのファイルの内容を \
辞書順で比較します。おまけの機能として、ファイル名と \
ファイルディスクリプタのどちらでも処理できます。\n");
		printf("使い方: %s {-fn=<ファイル名>|-fd=<ファイルディスクリプタ>} {-fn=<ファイル名>|-fd=<ファイルディスクリプタ>}\n", argv[0]);
		return EXIT_FAILURE;
	}

	int fd1 = getfd(argv[1]);
	int fd2 = getfd(argv[2]);
	if(fd1 == -1 || fd2 == -1)
	{
		printf("使い方: %s {-fn=<ファイル名>|-fd=<ファイルディスクリプタ>} {-fn=<ファイル名>|-fd=<ファイルディスクリプタ>}\n", argv[0]);
		return EXIT_FAILURE;
	}
	if(fd1 == 0 || fd2 == 0)
	{
		printf("fd 引数が不正です。\n");
		printf("(STDIN の利用に関するバグを、まだ修正中です。)\n");
		printf("使い方: %s {-fn=<ファイル名>|-fd=<ファイルディスクリプタ>} {-fn=<ファイル名>|-fd=<ファイルディスクリプタ>}\n", argv[0]);
		return EXIT_FAILURE;
	}

	struct fileComp* fc = comparefds(fd1, fd2);
	
	printf(
			"辞書順の比較: \"%s\" は %s \"%s\"\n", 
			securityCheck(argv[1], fc->fileContents1),
			fc->cmp > 0 ? "後 (after)" : (fc->cmp < 0 ? "前 (before)" : "等しい (equivalent to)"),
			securityCheck(argv[2], fc->fileContents2));

	return EXIT_SUCCESS;
}
