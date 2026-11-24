#include <stdio.h>

/* gcc -z execstack -fno-stack-protector -o inject inject.c */

int main()
{
    char buffer[128];

    puts("俺たちの行く先には");
    puts("道なんて ... 必要ない。");
    gets(buffer);
    
    return 0;
}

