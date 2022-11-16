#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void NumToString(double num, char *str_num) {
  if (num < 0) {
    num = num * -1;
  }
  // Find biggest divisor
  double biggest_divisor = 1;
  while (1) {
    if (num / biggest_divisor < 10) {
      break;
    }
    biggest_divisor*=10;
  }
  // Digits to string
  int i = 0;
  while (biggest_divisor ) {
    if (num < 10 && num >=1) {
      int remain = num;
      str_num[i] = remain + 48;
      i++;
      break;
    }
    int mult = num / biggest_divisor;
    num = num - mult * biggest_divisor;
    biggest_divisor/=10;
    str_num[i] = mult + 48;
    i++;
  }
  str_num[i] = '\0';
}

void FloatToString(double num) {
  // int str_len = strlen(str_num);
  if (num < 0) {
    num = num * -1;
  }
  double flt = num - floorf(num);
  printf("%lf", flt);
}

int main() {
  char str[100] = {'\0'};
  float num = -13562.512345;
  NumToString(num, str);
  printf("%s\n", str);
  // printf("This is %#f;\n", -1111111.512345);
  FloatToString(num);
  return 0;
}