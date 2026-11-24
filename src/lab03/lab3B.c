#include <signal.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/prctl.h>
#include <wait.h>
#include "utils.h"

ENABLE_TIMEOUT(60)

/* gcc -z execstack -fno-stack-protector -o lab3B lab3B.c */

/* ヒント: .pass ファイルを開いて読み込むシェルコードを書くこと。
   ptrace() は、/bin/sh を起動するシェルコードを使わせないためのものです */

int main()
{
    pid_t child = fork();
    char buffer[128] = {0};
    int syscall = 0;
    int status = 0;

    if(child == 0)
    {
        prctl(PR_SET_PDEATHSIG, SIGHUP);
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);

        /* 気にする必要があるのはここだけです */
        puts("シェルコードをちょうだい、いいね?");
        gets(buffer);
    }
    else
    {
        /* 小さな exec() サンドボックス。ここは無視して構いません */
        while(1)
        {
            wait(&status);
            if (WIFEXITED(status) || WIFSIGNALED(status)){
                puts("子プロセスを終了します...");
                break;
            }

            /* システムコール番号を取得する */
            syscall = ptrace(PTRACE_PEEKUSER, child, 4 * ORIG_EAX, NULL);

            /* システムコール 11 (exec) を弾く */
            if(syscall == 11)
            {
                printf("exec() は使わせません\n");
                kill(child, SIGKILL);
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
