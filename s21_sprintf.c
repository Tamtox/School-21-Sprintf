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

// String number to int
int StringNumToInt(char *num) {
  int result = 0;
  int num_len = strlen(num);
  int multiplyer = 1;
  for (int i = num_len - 1; i >= 0; i--) {
    int new_num = multiplyer * (num[i] - 48);
    result = result + new_num;
    multiplyer*=10;
  }
  return result;
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
int SetWidth(specifierEntry *entry, int start_pos, char *spec) {
  int spec_len = strlen(spec);
  int end_pos = -1;
  char *width_chars = "0123456789*";
  char *allowed_specifiers = "cdifsugGeExXonp";
  char *length_chars = "hlL";
  char width_str[50] = {'\0'};
  int width_len = 0;
  int digits_count = 0;
  int star_count = 0;
  // Find end of width and check for unfit chars 
  for (int i = start_pos; i < spec_len; i++) {
    if (strchr(allowed_specifiers, spec[i]) || strchr(length_chars, spec[i]) || spec[i] == '.') {
      end_pos = i;
      break;
    }
    if (!strchr(width_chars, spec[i])) {
      fprintf(stderr, "Unknown conversion type character '%c' in format", spec[i]);
      exit(1);
    }
    if (strchr(width_chars, spec[i])) {
      if (spec[i] == '*') {
        star_count++;
      } else {
        digits_count++;
      }
    }
    width_len++;
  }
  SliceStr(spec, width_str, start_pos, end_pos);
  // Throw error if both digits and * are present
  if (digits_count && star_count) {
    PrintError("Unknown conversion type character '*' in format");
  }
  if (star_count) {
    // Throw error if more than one star
    if (star_count > 1) {
      PrintError("Unknown conversion type character '*' in format");
    } else {
      entry->width = -2;
    }
  }
  // Set width
  if (digits_count) {
    int width = StringNumToInt(width_str);
    printf("%d\n", width);
    entry->width = width;
  }
  return end_pos;
}

// Set precision
int SetPrecision(specifierEntry *entry, int start_pos, char *spec) {
  int spec_len = strlen(spec);
  int end_pos = -1;
  char *precision_chars = "0123456789*";
  char *allowed_specifiers = "cdifsugGeExXonp";
  char *length_chars = "hlL";
  char precision_str[50] = {'\0'};
  int precision_len = 0;
  int digits_count = 0;
  int star_count = 0;
  // Find end of precision and check for unfit chars 
  for (int i = start_pos; i < spec_len; i++) {
    if (strchr(allowed_specifiers, spec[i]) || strchr(length_chars, spec[i])) {
      end_pos = i;
      break;
    }
    if (!strchr(precision_chars, spec[i])) {
      fprintf(stderr, "Unknown conversion type character '%c' in format", spec[i]);
      exit(1);
    }
    if (strchr(precision_chars, spec[i])) {
      if (spec[i] == '*') {
        star_count++;
      } else {
        digits_count++;
      }
    }
    precision_len++;
  }
  SliceStr(spec, precision_str, start_pos, end_pos);
  // Throw error if both digits and * are present
  if (digits_count && star_count) {
    PrintError("Unknown conversion type character '*' in format");
  }
  if (star_count) {
    // Throw error if more than one star
    if (star_count > 1) {
      PrintError("Unknown conversion type character '*' in format");
    } else {
      entry->precision = -2;
    }
  }
  // Set precision to 1 if only dot is present
  if (precision_len == 0) {
    entry->precision = 1;
  }
  // Set precision
  if (digits_count) {
    int precision = StringNumToInt(precision_str);
    printf("%d\n", precision);
    entry->precision = precision;
  }
  return end_pos;
}

// Set length
// int SetLength (specifierEntry *entry, int start_pos, char *spec) {
//   int end_pos = -1;
//   return end_pos;
// }

// Read specifier and set its parameters
void ReadCheckSpecifier(char *spec, specifierEntry *entry ) {
  int spec_len = strlen(spec);
  char *allowed_chars = "+- #0123456789*.hlL";
  char *flags = "+- #0";
  char *width = "0123456789*";
  char *length = "hlL";
  int flag_mode = 1;
  int width_mode = 0;
  int precision_mode = 0;
  int length_mode = 0;
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
      if (width_mode) {
        if (spec[i] == '0') {
          PrintError("error: '0' flag ignored with precision and '%d' gnu_printf format");
        }
        int width_end = SetWidth(entry, i, spec);
        i = width_end - 1;
        width_mode = 0;
        precision_mode = 1;
        continue;
      } else {
        PrintError("error: too many arguments for format");
      }
    } else {
      width_mode = 0;
      precision_mode = 1;
    }
    // Check precision
    if(spec[i] == '.') {
      if (precision_mode) {
        int precision_end = SetPrecision(entry, i + 1, spec);
        i = precision_end - 1;
        precision_mode = 0;
        length_mode = 1;
        continue;
      } else {
        PrintError("too many arguments for format");
      }
    } else {
      precision_mode = 0;
      length_mode = 1;
    }
    // Check length
    if (strchr(length, spec[i])) {
      if (length_mode) {
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
      specifierEntry entry = {false,false,false,false,false,-1,-1,'n',str[spec_end - 1]};
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
  char *str = "This %12.5555123s";
  Sprintf(buff, str);
  return 0;
}