/*
 * 書式文字列の例 2
 *
 */
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
  int unchangeable = 0xcafebabe;
  char buf[100];

  fgets(buf, 100, stdin);
  printf(buf);

  //ヒント
  fprintf(stderr, "unchangeable @ %p\n", &unchangeable);

  //fgets を使えばバッファをオーバーフローさせることはできないが、
  //  念のため...
  if (unchangeable != 0xcafebabe) {
    printf("クッキーが上書きされました!\n");
    printf("unchangeable が %#x に変わりました\n", unchangeable);
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
