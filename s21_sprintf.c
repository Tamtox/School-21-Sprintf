#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_sprintf.h"

// Find where specifier ends (need to add the type check with argument)
int FindEndOfSpecifier(char *str, int start_pos, int str_len) {
  int end_pos = -1;
  char *allowed_specifiers = "cdifsugGeExXonp";
  int spec_len = strlen(allowed_specifiers);
  for (int i = start_pos; i < str_len; i++) {
    for (int j = 0; j < spec_len; j++) {
      if (str[i] == allowed_specifiers[j]) {
        end_pos = i;
        break;
      }
    }
    if (end_pos > 0) {
      break;
    }
  }
  return end_pos;
}

// Check specifier for unfit chars
void CheckSpec () {

}

// Print error
void PrintError (char *message) {
  fprintf(stderr, message);
  exit(1);
}

// Read and set flag
void SetFlag(specifierEntry *entry, char flag) {
  if (flag == '+') {
    if (entry->flag_plus) {
      PrintError("Repeated '+' flag in format");
    } else {
      entry->flag_plus = true;
    }
  } else if (flag == '-') {
    if (entry->flag_minus) {
      PrintError("Repeated '-' flag in format");
    } else {
      entry->flag_minus = true;
    }
  } else if (flag == ' ') {
    if (entry->flag_space) {
      PrintError("Repeated ' ' flag in format");
    } else {
      entry->flag_space = true;
    }
  } else if (flag == '#') {
    if (entry->flag_sharp) {
      PrintError("Repeated '#' flag in format");
    } else {
      entry->flag_sharp = true;
    }
  } else if (flag == '0') {
    if (entry->flag_zero) {
      PrintError("Repeated '0' flag in format");
    } else {
      entry->flag_zero = true;
    }
  } else {
    fprintf(stderr, "Unknown conversion type character '%c' in format", flag);
    exit(1);
  }
}

// Read specifier and set its parameters
void ReadSpecifier(char *spec, specifierEntry *entry ) {
  int spec_len = strlen(spec);
  // Set specifier type
  entry->type = spec[spec_len - 1];
  char *flags = "+- #0";
  int flag_mode = 1;
  int width_mode = 0;
  int precision_mode = 0;
  int length_mode = 0;
  for (int i = 0; i < spec_len; i ++) {
    // Chech unfit chars

    // Check flags
    if (flag_mode) {
      if (strchr(flags, spec[i])) {
        SetFlag(entry, spec[i]);
      } else {
        flag_mode = 0;
        width_mode = 1;
      }
    }
    // Check width
    if (spec[i] ) {

    }
    // Check precision
    if(spec[i] == '.') {
      fprintf(stderr, "Precision used with '%c' gnu_printf format");
      exit(1);
    } else {
      precision_mode = 0;
    }
    // Check length
    if (spec[i]) {

    } else {
      length_mode = 0;
      break; 
    }
  }
}

void Sprintf(char *buff, char *str, ...) {
  // int total_args = 0;
  va_list ap;
  va_start(ap, str);

  int buff_size = sizeof(buff);
  int str_len = strlen(str);
  printf("%d , %d", str_len, buff_size);

  // Parse str according to format :%[flags][width][.precision][length]specifier.
  char output[500] = {'\0'};
  char *allowed_specifiers = "cdifsugGeExXonp";
  for (int i = 0; i < str_len; i++) {
    // Detrmine the positions of specifier start and end
    if (str[i] == '%') {
      int spec_end = FindEndOfSpecifier(str, i + 1, str_len);
      if (spec_end < 1) {

      } 
    }
    // Compare the types of specifier and corresponding argument
    // Parse specifier
  }
  // return str_len;
}

int main() {
  char buff[20] = {'\0'};
  char *str = "abc";
  Sprintf(buff, str);
  // printf("%s,% dz\t", str, 6);
  return 0;
}