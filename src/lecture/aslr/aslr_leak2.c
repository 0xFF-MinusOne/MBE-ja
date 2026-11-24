#include <stdio.h>
#include <string.h>

/* gcc -pie -fPIE -fno-stack-protector -o aslr_leak2 ./aslr_leak2.c */

int main(int argc, char * argv[])
{
    char leaky[16];
    
    if(argc < 2)
    {
        printf("引数を 2 つ指定してください。\n");
        return 1;
    }

    memcpy(leaky, argv[1], 16);
    
    printf("漏れるバッファ: %s\n", leaky);
    printf("もう一回? : ");
    fgets(leaky, 64, stdin);

    return 0;
}
