#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // 1 %c spec
  char c = 'c';
  printf("%c;\n" ,c);
  // 2 %s spec
  char *str = "This";
  printf("%.s;\n", str);
  // 3 %d spec
  int num = 11;
  printf("%3.3d;\n", num);
  // 4 %i spec
  int num1 = -100;
  printf("%3.3i;\n", num1);
  // 5 %u spec
  int num2 = -33;
  printf("%3.3u;\n", num2);
  // 6 %f spec
  float flt = 8.7;
  printf("%20.3f;\n", flt);
  return 0;
}