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
    char length;
    char type;
} specifierEntry;


#endif // SRC_S21_SPRINTF_H_