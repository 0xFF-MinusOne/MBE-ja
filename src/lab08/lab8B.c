/*
 * gcc -z relro -z now -fPIE -pie -fstack-protector-all -o lab8B lab8B.c
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_FAVES 10

struct vector {
	void (*printFunc)(struct vector*);
	char a;
	short b;
	unsigned short c;
	int d;
	unsigned int e;
	long f;
	unsigned long g;
	long long h;
	unsigned long long i;
};

struct vector v1;
struct vector v2;
struct vector v3;
struct vector* faves[MAX_FAVES];

void printVector(struct vector* v);

void printMenu()
{
	printf("+------------------------------------------------------------+\n");
	printf("|                                                            |\n");
	printf("|  1. データを入力する                                    :> |\n");
	printf("|  2. ベクトルを合計する                                  :] |\n");
	printf("|  3. ベクトルを表示する                                  :3 |\n");
	printf("|  4. 合計をお気に入りに保存する                          8) |\n");
	printf("|  5. お気に入りを表示する                                :O |\n");
	printf("|  6. お気に入りを読み込む                                :$ |\n");
	printf("|  9. ヘルプを表示する                                    :D |\n");
	printf("|                                                            |\n");
	printf("+------------------------------------------------------------+\n");
	printf("コマンドを入力せよ: ");
}

struct vector* vectorSel()
{
	printf("どのベクトルですか? ");
	char sel;
	while((sel = getchar()) == '\n'); // C が大好きだ。
	switch(sel)
	{
		case '1':
			return &v1;
		case '2':
			return &v2;
		case '3':
			return &v3;
		default:
			printf("\nベクトルの選択が不正です\n");
			exit(EXIT_FAILURE);
	}
}

void enterData()
{
	struct vector* v = vectorSel();
	if(v == &v3)
	{
		printf("合計に手動でデータを入力しようとしないでください。\n");
		return;
	}
	printf("データ入力の時間です!\n");
	printf("char a: ");
	while((v->a = getchar()) == '\n'); // やっぱり C が大好きだ。
	printf("short b: ");
	scanf("%hd", &(v->b));
	printf("unsigned short c: ");
	scanf("%hu", &(v->c));
	printf("int d: ");
	scanf("%d", &(v->d));
	printf("unsigned int e: ");
	scanf("%u", &(v->e));
	printf("long f: ");
	scanf("%ld", &(v->f));
	printf("unsigned long g: ");
	scanf("%lu", &(v->g));
	printf("long long h: ");
	scanf("%lld", &(v->h));
	printf("unsigned long long i: ");
	scanf("%llu", &(v->i));
	v->printFunc = printVector;
}

void sumVectors()
{
	if(v1.a==0 || v2.a==0 ||
			v1.b==0 || v2.b==0 ||
			v1.c==0 || v2.c==0 ||
			v1.d==0 || v2.d==0 ||
			v1.e==0 || v2.e==0 ||
			v1.f==0 || v2.f==0 ||
			v1.g==0 || v2.g==0 ||
			v1.h==0 || v2.h==0 ||
			v1.i==0 || v2.i==0)
	{
		printf("加数すら設定していませんね... :(\n");
		return;
	}
	v3.a = v1.a + v2.a;
	v3.b = v1.b + v2.b;
	v3.c = v1.c + v2.c;
	v3.d = v1.d + v2.d;
	v3.e = v1.e + v2.e;
	v3.f = v1.f + v2.f;
	v3.g = v1.g + v2.g;
	v3.h = v1.h + v2.h;
	v3.i = v1.i + v2.i;
	printf("合計しました。\n");
}

/*
 * この関数を使わなければボーナスポイント。
 */
void thisIsASecret()
{
	system("/bin/sh");
}

void printVector(struct vector* v)
{
	printf("アドレス: %p\n", v);
	printf("void printFunc: %p\n", v->printFunc);
	printf("char a: %c\n", v->a);
	printf("short b: %hd\n", v->b);
	printf("unsigned short c: %hu\n", v->c);
	printf("int d: %d\n", v->d);
	printf("unsigned int e: %u\n", v->e);
	printf("long f: %ld\n", v->f);
	printf("unsigned long g: %lu\n", v->g);
	printf("long long h: %lld\n", v->h);
	printf("unsigned long long i: %llu\n", v->i);
}

void fave()
{
	unsigned int i;
	for(i=0; i<MAX_FAVES; i++)
		if(!faves[i])
			break;
	if(i == MAX_FAVES)
		printf("お気に入りが多すぎます。\n");
	else
	{
		faves[i] = malloc(sizeof(struct vector));
		memcpy(faves[i], (int*)(&v3)+i, sizeof(struct vector));
		printf("そのベクトルをお気に入りに追加したのですね。 \
でも、本当にお気に入りでしたか?\n");
	}
}

void printFaves()
{
	unsigned int i;
	for(i=0; i<MAX_FAVES; i++)
		if(faves[i])
			printVector(faves[i]);
		else
			break;
	printf("%u 個のベクトルを表示しました。\n", i);
}

void loadFave()
{
	printf("どのお気に入りですか? ");
	unsigned int i;
	scanf("%u", &i);
	if(i >= MAX_FAVES)
	{
		printf("インデックスが範囲外です\n");
		return;
	}

	struct vector* v = vectorSel();
	if(v == &v3)
	{
		printf("合計に手動でデータを入力しようとしないでください。\n");
		return;
	}
	memcpy(v, faves[i], sizeof(v));
}

void help()
{
	printf("\
このプログラムは、2 つのベクトルを足し合わせて、結果を 3 つ目のベクトルに保存します。 \
その合計をお気に入りのリストに追加したり、お気に入りを加数の一方に \
読み込み直したりできます。\n");
}

int main(int argc, char** argv)
{
	char sel;
	printMenu();
	v1.printFunc = printf;
	v2.printFunc = printf;
	v3.printFunc = printf;
	struct vector* v;
	while((sel = getchar()) && (sel == '\n' || getchar())) // 魔法 ;^)
	{
		if(sel == '\n')
			continue;

		switch(sel)
		{
			case '0':
				printf("では、さようなら。\n");
				return EXIT_SUCCESS;
			case '1':
				enterData();
				break;
			case '2':
				sumVectors();
				break;
			case '3':
				v = vectorSel();
				//printf("Calling %p\n", v->printFunc);
				v->printFunc(v);
				break;
			case '4':
				fave();
				break;
			case '5':
				printFaves();
				break;
			case '6':
				loadFave();
				break;
			case '9':
				help();
				break;
			default:
				printf("\n不正な入力です。 \
これを pwn しようとするあなたの無駄な試みと同じです。\n"); 
				return EXIT_FAILURE;
		}
		printMenu();
	}
	return EXIT_SUCCESS;
}
