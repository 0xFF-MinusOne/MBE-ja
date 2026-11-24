#include <stdio.h>
#include <string.h>
#define STDIN 0
//gcc -fstack-protector-all -mpreferred-stack-boundary=2 -o canary canary.c

void contrivedLeak() {
    char buf[512];
    scanf("%s", buf);
    printf(buf);
    return;
}

void contrivedOverflow() {
    printf("\n..読書が好きなんです ^_^ <==  ");
    char buf[24];
    read(STDIN, buf, 48);
    char lolz[24];
    strcpy(lolz, buf);
    printf(lolz);
    return;
}

int main(int argc, char* argv[]) {
    setbuf(stdout, NULL);
    printf("与えたものが返ってきます\n");
    printf("==> 無用の長物: ");
    contrivedLeak();
    printf("\nあなたのラッキーナンバーは 512 と 1024 です\n");
    contrivedOverflow();
    printf("\n\nおっと、待った\n");
    return 0;

}
