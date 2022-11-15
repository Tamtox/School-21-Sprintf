#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // // 1 %c spec
  // char c = 'c';
  // printf("This is char:%-5c;\n" ,c);
  // // 2 %s spec
  // char *str = "This";
  // printf("This is string: %5.10s;\n", str);
  // 3 %d spec
  int num = 1234;
  printf("This is d num:%7d;\n", num);
  // 4 %i spec
  int num1 = 100;
  printf("This is i num:%+5.4i;\n", num1);
  // 5 %u spec
  int num2 = 33;
  printf("This is u num:%5.4u;\n", num2);
  // // 6 %f spec
  // float flt = 8.7;
  // printf("This is float:%20.3f;\n", flt);
  return 0;
}