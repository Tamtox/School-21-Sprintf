#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_sprintf.h"

// Slice str part 
void SliceStr(char *str, char *result, int from, int to) {
  for (int i = 0; i < to - from; i++) {
    result[i] = str[from + i];
  }
  result[to - from] = '\0';
}

// Find where specifier ends (need to add the type check with argument)
int FindEndOfSpecifier(char *str, int start_pos, int str_len) {
  int end_pos = -1;
  char *allowed_specifiers = "cdifsugGeExXonp";
  for (int i = start_pos; i < str_len; i++) {
    if (strchr(allowed_specifiers, str[i])) {
      end_pos = i + 1;
      break;
    }
    if (end_pos > 0) {
      break;
    }
  }
  return end_pos;
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
  }
}

// Set width
int SetWidth (specifierEntry *entry, int start_pos, char *spec) {
  int end_index = -1;
}

// Set precision
int SetPrecision (specifierEntry *entry, int start_pos, char *spec) {
  int end_index = -1;
  char *precision = ".123456789*";
  for (int i = start_pos; i < str_len; i++) {
    if (strchr(allowed_specifiers, str[i])) {
      end_pos = i + 1;
      break;
    }
    if (end_pos > 0) {
      break;
    }
  }
}

// Set length
int SetLength (specifierEntry *entry, int start_pos, char *spec) {
  int end_index = -1;
}

// Read specifier and set its parameters
void ReadCheckSpecifier(char *spec, specifierEntry *entry ) {
  int spec_len = strlen(spec);
  char *allowed_chars = "+- #0123456789*.hlL";
  char *flags = "+- #0";
  char *width = "123456789*";
  char *length = "hlL";
  int flag_mode = 1;
  int width_mode = 0;
  int precision_mode = 0;
  int length_mode = 0;
  printf("%s\n", spec);
  printf("%c\n", entry->type);
  for (int i = 1; i < spec_len - 1; i ++) {
    // Check unfit chars
    if (!strchr(allowed_chars, spec[i])) {
      fprintf(stderr, "Unknown conversion type character '%c' in format", spec[i]);
      exit(1);
    }
    // Check flags
    if (strchr(flags, spec[i])) {
      if (flag_mode) {
        SetFlag(entry, spec[i]);
      } else {
        PrintError("too many arguments for format");
      }
    } else {
      flag_mode = 0;
      width_mode = 1;
    }
    // Check width
    if (strchr(width, spec[i])) {

    }
    // Check precision
    if(spec[i] == '.') {
      if (precision_mode) {
        SetPrecision
      } else {
        PrintError("too many arguments for format");
      }
    }
    // Check length
    if (strchr(length, spec[i])) {
      if (length_mode) {
        if (spec[i])
      } else {
        PrintError("too many arguments for format");
      }
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
  if(buff[0]) {

  }
  int str_len = strlen(str);
  // Parse str according to format :%[flags][width][.precision][length]specifier.
  // char output[500] = {'\0'};
  for (int i = 0; i < str_len; i++) {
    // Detrmine the positions of specifier start and end
    if (str[i] == '%') {
      int spec_end = FindEndOfSpecifier(str, i + 1, str_len);
      // Copy specifier
      specifierEntry entry = {false,false,false,false,false,{'\0'},{'\0'},'n',str[spec_end - 1]};
      char specifier[100] = {'\0'};
      SliceStr(str, specifier, i, spec_end);
      ReadCheckSpecifier(specifier, &entry);
      i = spec_end - 1;
    }
  }
  // return str_len;
}

int main() {
  char buff[20] = {'\0'};
  char *str = "This %2.5s%+&2d";
  Sprintf(buff, str);
  return 0;
}