#ifndef SRC_S21_SPRINTF_H_
#define SRC_S21_SPRINTF_H_

#include <stdbool.h>

typedef struct SpecifierEntry {
    bool flag_plus;
    bool flag_minus;
    bool flag_space;
    bool flag_sharp;
    bool flag_zero;
    int width;
    int precision;
    int length_h;
    int length_l;
    int length_L;
    char type;
} specifierEntry;

int s21_sprintf(char *buff, char *str, ...);


#endif // SRC_S21_SPRINTF_H_