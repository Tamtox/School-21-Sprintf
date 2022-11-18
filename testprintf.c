#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main() {
  char buff[100] = {'\0'};
  int res = sprintf(buff, "%7.10f", 11.123456);
  printf("%d\n", res);
  printf("%s\n", buff);
  char buf[80];
  gcvt (11.12345601, 8, buf);
  printf("%s", buf);
  return 0;
}
