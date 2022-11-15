#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // 1 %c spec
  // char c = '*';
  // printf("This is char:%5c;\n" ,c);
  // // 2 %s spec
  // char *str = "This";
  // printf("This is string: %5.10s;\n", str);
  // 3 %d spec
  char buff[50] = {'\0'};
  int num = 69;
  // char *y = "%0.*i;\n";
  sprintf( buff, "%.*i;\n" , num, num);
  // 4 %i spec
  // int num1 = 123;
  // printf("This is i num:%.5i;\n", num1);
  // // 5 %u spec
  // int num2 = -5;
  // printf("This is u num:%7.5u;\n", num2);
  // // 6 %f spec
  // float flt = 8.7;
  // printf("This is float:%20.3f;\n", flt);
  return 0;
}