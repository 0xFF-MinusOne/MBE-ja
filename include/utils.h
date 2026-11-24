/*
 * アンチデバッグ/アンチ逆アセンブル用のツール
 */

/* esp の解析を狂わせて、hexrays を妨害する */
#define deathrays \
    __asm__ volatile("push     %eax      \n"\
                     "xor      %eax, %eax\n"\
                     "jz       .+5       \n"\
                     ".word    0xC483    \n"\
                     ".byte    0x04      \n"\
                     "pop      %eax      \n");

/* シェルコードを避けるため、argv をクリアする */
#define clear_argv(_argv) \
    for (; *_argv; ++_argv) { \
        memset(*_argv, 0, strlen(*_argv)); \
    }
#define clear_envp(_envp) clear_argv(_envp)

/* ファイルディスクリプタの入出力バッファリングを無効にする */
#define disable_buffering(_fd) setvbuf(_fd, NULL, _IONBF, 0)

/* 改行まで stdin を読み捨てる */
void clear_stdin(void)
{
    char x = 0;
    while(1)
    {
        x = getchar();
        if(x == '\n' || x == EOF)
            break;
    }
}

/* stdin から数値を取得し、後始末もする */
unsigned int get_unum(void)
{
    unsigned int res = 0;
    fflush(stdout);
    scanf("%u", &res);
    clear_stdin();
    return res;
}

void prog_timeout(int sig)
{
  asm("mov $1, %eax;"
      "mov $1, %ebx;"
      "int $0x80");
}

#include <signal.h>
#define ENABLE_TIMEOUT(_time) \
  __attribute__ ((constructor)) void enable_timeout_cons() \
  { \
      signal(SIGALRM, prog_timeout); \
      alarm(_time); \
  }
