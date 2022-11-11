#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char c = 'c';
  printf("%c\n" ,c);
  char *str = "This";
  printf("%.10s\n", str);
  int num = 123;
  printf("%d\n", num);
  float flt = 8.7;
  printf("%.1f\n", flt);
  return 0;
}