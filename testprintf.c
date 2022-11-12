#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // %c spec
  char c = 'c';
  printf("%c\n" ,c);
  // %s spec
  char *str = "This";
  printf("%3.3s\n", str);
  // %d spec
  int num = 123;
  printf("%3.3d\n", num);
  // %i spec
  int num1 = -100;
  printf("%3.3i\n", num1);
  // %u spec
  int num2 = 33;
  printf("%3.3u\n", num2);
  // %f spec
  float flt = 8.7;
  printf("%3.3f\n", flt);
  return 0;
}