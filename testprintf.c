#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char c = 'c';
  printf("%c\n" ,c);
  char *str = "This";
  printf("%s\n", str);
  int num = 5;
  printf("%d\n", num);
  float flt = 8.7;
  printf("%.1f\n", flt);
  return 0;
}