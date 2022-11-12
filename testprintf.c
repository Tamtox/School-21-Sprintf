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
  printf("%010d\n", num);
  // %i spec
  int num1 = -100;
  printf("%i\n", num1);
  // %u spec
  int num2 = 33;
  printf("%u\n", num2);
  // %f spec
  float flt = 8.7;
  printf("%10.1f\n", flt);
  char buff[50] = {'\0'};
  char * toCpy = "Testing %10s\n\n";
  sprintf(buff, toCpy, "abc");
  printf("%s", buff);
  return 0;
}