#include <cstdlib>
#include <cstdio>
#include <cstring>

class Foo {
public:
  virtual void greet() { printf("Foo は「やあ」と言っています\n"); }
  void me() { printf("Foo が最高です\n"); }
};

class Bar : public Foo {
public:
  virtual void greet() { printf("Bar は「やあ」と言っています\n"); }
  virtual void dismiss() { printf("Bar は「さようなら」と言っています\n"); }
  void me() { printf("Bar が最高です\n"); }
};

class Baz : public Bar {
public:
  void dismiss() { printf("Baz は「さようなら」と言っています \n"); }
  void me() { printf("Baz が最高です\n"); }
};

int
main(int argc, char *argv[])
{
  Foo foo;
  Bar bar;
  Baz baz;
  Bar *barbaz = (Bar *)new Baz;

  foo.greet();
  puts("\n");

  bar.greet();
  bar.me();
  bar.dismiss();
  puts("\n");

  barbaz->me();
  barbaz->dismiss();

  return 0;
}

