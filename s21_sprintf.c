#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_sprintf.h"

// Print specifier struct for testing purposes
void PrintSpecifier(specifierEntry *entry) {
  printf("Flag minus: %d\n", entry->flag_minus ? 1 : 0);
  printf("Flag plus: %d\n", entry->flag_plus ? 1 : 0);
  printf("Flag space: %d\n", entry->flag_space ? 1 : 0);
  printf("Flag sharp: %d\n", entry->flag_sharp ? 1 : 0);
  printf("Flag zero: %d\n", entry->flag_zero ? 1 : 0);
  printf("Width is :%d\n", entry->width);
  printf("Precision is :%d\n", entry->precision);
  printf("Length h is :%d\n", entry->length_h);
  printf("Length l is :%d\n", entry->length_l);
  printf("Length L is :%d\n", entry->length_L);
  printf("Type is :%c\n", entry->type);
  printf("\n");
}

// Find length to print 

// Copy %s specifier into buffer
void CpyFormattedStrSpecifier(char *buff, int *buffPos, specifierEntry *entry, char *str) {
  int str_len = strlen(str);
  // Truncate string length with precision
  if (entry->precision >= 0 && entry->precision < str_len) {
    str_len = entry->precision;
  }
  // Set length to copy
  int to_print = str_len;
  if (entry->width > 0 && entry->width > str_len) {
    to_print = entry->width;
  }
  // Write string to buffer depending on minus flag
  int j = 0;
  for (int i = 0; i < to_print; i++) {
    if (entry->flag_minus) {
      if (i < str_len) {
      buff[*buffPos] = str[i];
      } else {
        buff[*buffPos] = ' ';
      }
    } else {
      if (i < to_print - str_len) {
        buff[*buffPos] = ' ';
      } else {
        buff[*buffPos] = str[j];
        j+=1;
      }
    }
    *buffPos = *buffPos + 1;
  }
}

// Slice str part 
void SliceStr(char *str, char *result, int from, int to) {
  for (int i = 0; i < to - from; i++) {
    result[i] = str[from + i];
  }
  result[to - from] = '\0';
}

// String num to num
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

// Num to string num

// Print error
void PrintError (char *message) {
  fprintf(stderr, message);
  exit(1);
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

// Read and set flag
void SetFlag(specifierEntry *entry, char flag) {
  // Flag plus
  if (flag == '+') {
    char *not_compatible = "csu";
    if (strchr(not_compatible, entry->type)) {
      fprintf(stderr, "error: '+' flag used with '%c' gnu_printf format", entry->type);
      exit(1);
    }
    // Flag ' ' is ignored when + is present
    if (entry->flag_space) {
      PrintError("error: ' ' flag ignored with '+' flag in gnu_printf format");
    }
    if (entry->flag_plus) {
      PrintError("error: repeated '+' flag in format");
    } else {
      entry->flag_plus = true;
    }
  } 
  // flag minus
  else if (flag == '-') {
    // Flag 0 is ignored when - is present
    if (entry->flag_zero) {
      PrintError("error: '0' flag ignored with '-' flag in gnu_printf format");
    }
    if (entry->flag_minus) {
      PrintError("error: repeated '-' flag in format");
    } else {
      entry->flag_minus = true;
    }
  } 
  // Flag space
  else if (flag == ' ') {
    char *not_compatible = "csu";
    if (strchr(not_compatible, entry->type)) {
      fprintf(stderr, "error: '+' flag used with '%c' gnu_printf format", entry->type);
      exit(1);
    }
    // Flag ' ' is ignored when + is present
    if (entry->flag_plus) {
      PrintError("error: ' ' flag ignored with '+' flag in gnu_printf format");
    }
    if (entry->flag_space) {
      PrintError("error: repeated ' ' flag in format");
    } else {
      entry->flag_space = true;
    }
  } 
  // Flag sharp
  else if (flag == '#') {
    char *not_compatible = "csdiu";
    if (strchr(not_compatible, entry->type)) {
      fprintf(stderr, "error: '#' flag used with '%c' gnu_printf format", entry->type);
      exit(1);
    }
    if (entry->flag_sharp) {
      PrintError("error: repeated '#' flag in format");
    } else {
      entry->flag_sharp = true;
    }
  }
  // Flag zero
   else if (flag == '0') {
    char *not_compatible = "cs";
    if (strchr(not_compatible, entry->type)) {
      fprintf(stderr, "error: '+' flag used with '%c' gnu_printf format", entry->type);
      exit(1);
    }
    // Flag 0 is ignored when - is present
    if (entry->flag_minus) {
      PrintError("error: '0' flag ignored with '-' flag in gnu_printf format");
    }
    if (entry->flag_zero) {
      PrintError("error: repeated '0' flag in format");
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
      fprintf(stderr, "error: unknown conversion type character '%c' in format", spec[i]);
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
    PrintError("error: unknown conversion type character '*' in format");
  }
  if (star_count) {
    // Throw error if more than one star
    if (star_count > 1) {
      PrintError("error: unknown conversion type character '*' in format");
    } else {
      entry->width = -2;
    }
  }
  // Set width
  if (digits_count) {
    int width = StringNumToInt(width_str);
    entry->width = width;
  }
  return end_pos;
}

// Set precision
int SetPrecision(specifierEntry *entry, int start_pos, char *spec) {
  // Check incompatible flags
  char *types = "diu";
  if (strchr(types, entry->type)) {
    if (entry->flag_zero) {
      fprintf(stderr, "error: '0' flag ignored with precision and '%c' gnu_printf format", entry->type);
      exit(1);
    }
  }
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
      fprintf(stderr, "error: unknown conversion type character '%c' in format", spec[i]);
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
    PrintError("error: unknown conversion type character '*' in format");
  }
  if (star_count) {
    // Throw error if more than one star
    if (star_count > 1) {
      PrintError("error: unknown conversion type character '*' in format");
    } else {
      entry->precision = -2;
    }
  }
  // Set precision to 1 if only dot is present
  if (precision_len == 0) {
    if (entry->type == 's') {
      entry->precision = 0;
    } else {
      entry->precision = 1;
    }
  }
  // Set precision
  if (digits_count) {
    int precision = StringNumToInt(precision_str);
    entry->precision = precision;
  }
  return end_pos;
}

// Set length
int SetLength (specifierEntry *entry, int start_pos, char *spec) {
  int spec_len = strlen(spec);
  int end_pos = -1;
  char *allowed_specifiers = "cdifsugGeExXonp";
  char *length_chars = "hlL";
  // Find end of length and unfit chars then set length chars
  for (int i = start_pos; i < spec_len; i++) {
    if (strchr(allowed_specifiers, spec[i])) {
      end_pos = i;
      break;
    }
    if (strchr(length_chars, spec[i])) {
      if (spec[i] == 'h') {
        entry->length_h = entry->length_h + 1;
      } else if (spec[i] == 'l') {
        entry->length_l = entry->length_l + 1;
      } else if (spec[i] == 'L') {
        entry->length_L = entry->length_L + 1;
      }
    } else {
      fprintf(stderr, "error: Unknown conversion type character '%c' in format", spec[i]);
      exit(1);
    }
  }
  // Check length for errors
  if (entry->length_L > 0) {
    if ((entry->length_l || entry->length_h) || entry->length_L > 1) {
      PrintError("error: too many arguments for format");
    }
  }
  if (entry->length_h > 0) {
    if ((entry->length_l || entry->length_L) || entry->length_h > 2) {
      PrintError("error: too many arguments for format");
    }
  }
  if (entry->length_l > 0) {
    if ((entry->length_h || entry->length_h) || entry->length_l > 2) {
      PrintError("error: too many arguments for format");
    }
  }
  // Check length for types
  if (entry->length_h) {
    char *allowed_types = "idouxX";
    if (!strchr(allowed_types, entry->type)) {
      PrintError("error: h only applies to integer specifiers: i, d, o, u, x and X");
    }
  } else if (entry->length_l) {
    char *allowed_types = "idouxXcs";
    if (!strchr(allowed_types, entry->type)) {
      PrintError("error: l only applies to integer specifiers: i, d, o, u, x and X as well as wide char and wide string");
    }
  } else if (entry->length_L) {
    char *allowed_types = "eEfgG";
    if (!strchr(allowed_types, entry->type)) {
      PrintError("error: L only applies to floating point specifiers: e, E, f, g and G");
    }
  }
  return end_pos;
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
  for (int i = 1; i < spec_len - 1; i ++) {
    // Check unfit chars
    if (!strchr(allowed_chars, spec[i])) {
      fprintf(stderr, "error: unknown conversion type character '%c' in format", spec[i]);
      exit(1);
    }
    // Check flags
    if (strchr(flags, spec[i])) {
      if (flag_mode) {
        SetFlag(entry, spec[i]);
        continue;
      } else {
        PrintError("error: too many arguments for format");
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
        PrintError("error: too many arguments for format");
      }
    } else {
      precision_mode = 0;
      length_mode = 1;
    }
    // Check length
    if (strchr(length, spec[i])) {
      if (length_mode) {
        int length_end = SetLength(entry, i, spec);
        i = length_end - 1;
        break;
      } else {
        PrintError("error: too many arguments for format");
      }
    } else {
      length_mode = 0;
      break; 
    }
  }
}

void Sprintf(char *buff, char *str, ...) {
  if (buff) {}
  va_list ap;
  va_start(ap, str);
  int str_len = strlen(str);
  // Parse str according to format :%[flags][width][.precision][length]specifier.
  char placeholder[500] = {'\0'};
  int buffPos = 0;
  for (int i = 0; i < str_len; i++) {
    // Detrmine the positions of specifier start and end then copy output to buffer
    if (str[i] == '%') {
      int spec_end = FindEndOfSpecifier(str, i + 1, str_len);
      // Copy specifier
      specifierEntry entry = {false,false,false,false,false,-1,-1,0,0,0,str[spec_end - 1]};
      char specifier[100] = {'\0'};
      SliceStr(str, specifier, i, spec_end);
      ReadCheckSpecifier(specifier, &entry);
      // Set width from argument
      if (entry.width == -2) {
        int arg_width = va_arg(ap, int);
        entry.width = arg_width;
      }
      // Set precision from argument
      if (entry.precision == -2) {
        int arg_precision = va_arg(ap, int);
        entry.precision = arg_precision;
      }
      // PrintSpecifier(&entry);
      if (entry.type == 'd' || entry.type == 'i') {
        int arg = va_arg(ap, int);
        printf("%d\n", arg);
      } else if (entry.type == 'c') {
        int arg = va_arg(ap, int);
        printf("%c\n", arg);
      } else if (entry.type == 's') {
        char *arg = va_arg(ap, char *);
        CpyFormattedStrSpecifier(placeholder, &buffPos, &entry, arg);
      }
      i = spec_end - 1;
      continue;
    }
    placeholder[buffPos] = str[i];
    buffPos++;
  }
  // Copy contents of placeholder into buffer
  int i = 0;
  while (placeholder[i] != '\0') {
    buff[i] = placeholder[i];
    i++;
  }
  buff[i] = '\0';
}

int main() {
  char buff[500] = {'\0'};
  char *str = "This is %.s;";
  Sprintf(buff, str, "Test");
  printf("%s", buff);
  return 0;
}