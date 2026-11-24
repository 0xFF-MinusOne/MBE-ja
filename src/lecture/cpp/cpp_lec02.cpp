/*
 *    出典: https://defuse.ca/exploiting-cpp-vtables.htm
 */

#include <cstdlib>
#include <cstdio>
#include <cstring>

class CodeTester {
    public:
        virtual void test(const char*code)
        {
          int (*ret)() = (int(*)())code;
          ret();
        }
};

class CommandExecutor {
    public:
        virtual void execute(const char *command)
        {
            system(command);
        }
};

class Greeter {
    public:
        virtual void sayHello(const char *name)
        {
            printf("こんにちは、%s!\n", name);
        }
};

void greet(Greeter *greeter);
void doNothing();

int main(int argc, char **argv)
{
    Greeter g;
    char buf[64];
    printf("このプログラムを使うには 18 歳以上でなければなりません。あなたは 18 歳以上ですか? ");

    doNothing();

    gets(buf); /* オーバーフロー */
    if (strcmp(buf, "y") == 0) {
        greet(&g);
        return 0;
    } else {
        return 1;
    }
}

void greet(Greeter *greeter)
{
    char name[100];
    printf("お名前は? ");
    fgets(name, 100, stdin);
    name[strlen(name)-1] = '\0';
    greeter->sayHello(name);
}

/* これは、コンパイラ/リンカに CommandExecutor のコードを含めさせるためだけに
 * ある。そうしないと、そのコードは使われないままになってしまう。 */
void doNothing()
{
    CommandExecutor e;
    CodeTester c;
}

