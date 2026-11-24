/*
 * 書式文字列の例 3
 *
 */
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
  int unchangeable = 0xdeadbeef;
  char buf[100];

  fgets(buf, 100, stdin);
  printf(buf);

  //ヒント
  fprintf(stderr, "unchangeable @ %p\n", &unchangeable);

  if (unchangeable == 0xcafebabe) {
    printf("成功!\n");
  } else {
    printf("失敗!\n");
    printf("unchangeable は %#x です", unchangeable);
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
