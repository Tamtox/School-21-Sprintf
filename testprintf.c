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
  printf("%s\n", str);
  // %d spec
  int num = -123;
  printf("%d\n", num);
  // %i spec
  int num1 = -100;
  printf("%i\n", num1);
  // %f spec
  float flt = 8.7;
  printf("%10.1f\n", flt);
  return 0;
}