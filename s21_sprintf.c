#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "s21_sprintf.h"

// Unshift into string
void UnshiftChar(char *str, char c) {
  int str_len  = strlen(str);
  for (int i = str_len; i > 0; i--) {
    str[i] = str[i - 1];
  }
  str[0] = c;
  str[str_len + 1] = '\0';
}

// Slice str part 
void SliceStr(char *str, char *result, int from, int to) {
  for (int i = 0; i < to - from; i++) {
    result[i] = str[from + i];
  }
  result[to - from] = '\0';
}

// Reverse string
void ReverseStr(char *str) {
  int str_len = strlen(str);
  for (int i = 0; i < str_len / 2; i++) {
    char c = str[i];
    str[i] = str[str_len - 1 - i];
    str[str_len - 1 - i] = c;
  }
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

void NumToString(double num, char *str_num) {
  printf("%lf\n", num);
  if (num < 0) {
    num = num * -1;
  }
  int i = 0;
  while (1) {
    if (num < 10) {
      int x = num;
      str_num[i] = x + 48;
      break;
    }
    double div = 10.0;
    int x = fmod(num, div);
    str_num[i] = x + 48;
    i++;
    num = num / div;
  }
  str_num[i + 1] = '\0';
  ReverseStr(str_num);
}

// Float to string
void FloatToString (double num, char *str_num, int precision) {
  if (num < 0) {
    num = num * -1;
  }
  // Check if float is less than 1
  int less_than_one = num < 1 ? 1 : 0;
  if (less_than_one) {
    double one = 1.0;
    num = num + one;
  }
  // Float adjustment
  for (int i = 0; i < precision; i++) {
    num *= 10;
  }
  // Number into string
  char placeholder[100] = {'\0'};
  NumToString(num, placeholder); 
  int p_len = strlen(placeholder);
  // Separate whole and float
  int i = 0;
  while (i < p_len - precision) {
    str_num[i] = placeholder[i];
    i++;
  }
  if (precision > 0) {
    str_num[p_len - precision] = '.';
    while (i < p_len) {
      str_num[i + 1] = placeholder[i];
      i++;
    }
    str_num[i + 1] = '\0';
  } else {
    str_num[i] = '\0';
  }
  if (less_than_one) {
    str_num[0] = '0';
  }
}

// Print error
void PrintError (char *message) {
  fprintf(stderr, message);
  exit(1);
}

// Copy %c specifier into buffer
void CpyFormattedCharSpecifier(char *buff, int *buffPos, specifierEntry *entry, char c) {
  int to_print = 1;
  if (entry->width > 0) {
    to_print = entry->width;
  }
  // Write char to buffer depending on minus flag
  int startPos = *buffPos;
  for (int i = 0; i < to_print; i++) {
    buff[*buffPos] = ' ';
    *buffPos = *buffPos + 1;
  }
  if (entry->flag_minus) {
    buff[startPos] = c;
  } else {
    buff[*buffPos - 1] = c;
  }
}

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
  int diff = to_print - str_len;
  if (entry->flag_minus) {
    for (int i = 0;i < str_len; i++) {
      buff[*buffPos] = str[i];
      *buffPos = *buffPos + 1;
    }
    for (int i = 0;i < diff; i++) {
      buff[*buffPos] = ' ';
      *buffPos = *buffPos + 1;
    }
  } else {
    for (int i = 0;i < diff; i++) {
      buff[*buffPos] = ' ';
      *buffPos = *buffPos + 1;
    }
    for (int i = 0;i < str_len; i++) {
      buff[*buffPos] = str[i];
      *buffPos = *buffPos + 1;
    }
  }
}

// Copy d, i and u specifiers into buffer
void CpyFormattedIntSpecifier(char *buff, int *buffPos, specifierEntry *entry, char *str_num, int positive) {
  int str_len  = strlen(str_num);
  // Apply precision
  if (entry->precision >= 0) {
    if (str_num[0] == '0' && entry->precision == 0) {
      str_num[0] = '\0';
      str_len = 0;
    }
    int diff = entry->precision - str_len;
    if (diff > 0) {
      for(int i = 0; i < diff; i++) {
        UnshiftChar(str_num, '0');
      }
      str_len = entry->precision;
    }
    // Set preceding minus , plus or empty space
    if (!positive || entry->flag_plus) {
      if (!positive) {
        UnshiftChar(str_num, '-');
        str_len++;
      }
      if (entry->flag_plus && positive) {
        UnshiftChar(str_num, '+');
        str_len++;
      }
    } else {
      if (entry->flag_space) {
        UnshiftChar(str_num, ' ');
        str_len++;
      }
    }
  }
  // Set length to copy
  int to_print = str_len;
  if (entry->width > str_len) {
    to_print = entry->width;
  }
  int diff = to_print - str_len;
  if (entry->flag_zero) {
    for (int i = 0; i < diff; i++) {
      UnshiftChar(str_num, '0');
    }
    // Set preceding minus , plus or empty space
    if (!positive || entry->flag_plus) {
      if (!positive) {
        str_num[0] = '-';
      }
      if (entry->flag_plus && positive) {
        str_num[0] = '+';
      }
    } else {
      if (entry->flag_space) {
        str_num[0] = ' ';
      }
    }
    int i = 0;
    while (str_num[i] != '\0') {
      buff[*buffPos] = str_num[i];
      *buffPos = *buffPos + 1;
      i++;
    }
  } else {
    if (entry->precision < 0) {
      // Set preceding minus , plus or empty space
      if (!positive || entry->flag_plus) {
        if (!positive) {
          UnshiftChar(str_num, '-');
          str_len++;
          diff--;
        }
        if (entry->flag_plus && positive) {
          UnshiftChar(str_num, '+');
          str_len++;
          diff--;
        }
      } else {
        if (entry->flag_space) {
          UnshiftChar(str_num, ' ');
          str_len++;
          diff--;
        }
      }
    }
    if (entry->flag_minus) {
      for (int i = 0;i < str_len; i++) {
        buff[*buffPos] = str_num[i];
        *buffPos = *buffPos + 1;
      }
      for (int i = 0;i < diff; i++) {
        buff[*buffPos] = ' ';
        *buffPos = *buffPos + 1;
      }
    } else {
      for (int i = 0;i < diff; i++) {
        buff[*buffPos] = ' ';
        *buffPos = *buffPos + 1;
      }
      for (int i = 0;i < str_len; i++) {
        buff[*buffPos] = str_num[i];
        *buffPos = *buffPos + 1;
      }
    }
  }
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
  // Check incompatible specifiers
  if (entry->type == 'c') {
    fprintf(stderr, "error: precision used with '%c' gnu_printf format", entry->type);
    exit(1);
  }
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
    if (entry->type == 's' || entry->type == 'f') {
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
    if ((entry->length_l || entry->length_L) || entry->length_h > 1) {
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

int s21_sprintf(char *buff, char *str, ...) {
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
      char str_num[100] = {'\0'};
      if (entry.type == 'd' || entry.type == 'i') {
        if (entry.length_l == 1) {
          long int arg = va_arg(ap, long int);
          NumToString(arg, str_num);
          int positive = arg >= 0 ? 1 : 0; 
          CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, positive);
        } else if (entry.length_l == 2) {
          long long int arg = va_arg(ap, long long int);
          NumToString(arg, str_num);
          int positive = arg >= 0 ? 1 : 0; 
          CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, positive);
        } else if (entry.length_h == 1) {
          short int arg = va_arg(ap, int);
          NumToString(arg, str_num);
          int positive = arg >= 0 ? 1 : 0; 
          CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, positive);
        } else {
          int arg = va_arg(ap, int);
          NumToString(arg, str_num);
          int positive = arg >= 0 ? 1 : 0; 
          CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, positive);
        }
      } else if (entry.type == 'u') {
        if (entry.length_l == 1) {
          long unsigned int arg = va_arg(ap, long unsigned int);
          NumToString(arg, str_num);
          CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, 1);
        } else if (entry.length_l == 2) {
          long long unsigned int arg = va_arg(ap, long long unsigned int);
          NumToString(arg, str_num);
          CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, 1);
        } else if (entry.length_h == 1) {
          short unsigned int arg = va_arg(ap, unsigned int);
          NumToString(arg, str_num);
          CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, 1);
        } else {
          unsigned int arg = va_arg(ap, unsigned int);
          NumToString(arg, str_num);
          CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, 1);
        }
      } else if (entry.type == 'c') {
        int arg = va_arg(ap, int);
        CpyFormattedCharSpecifier(placeholder, &buffPos, &entry, arg);
      } else if (entry.type == 's') {
        char *arg = va_arg(ap, char *);
        CpyFormattedStrSpecifier(placeholder, &buffPos, &entry, arg);
      } else if (entry.type == 'f') {
        double arg = va_arg(ap, double);
        int precision = entry.precision;
        entry.precision = -1;
        if (precision > 9) {
          precision = 9;
        } else if (precision < 0) {
          precision = 6;
        }
        int positive = arg >= 0 ? 1 : 0; 
        FloatToString (arg, str_num, precision);
        CpyFormattedIntSpecifier(placeholder, &buffPos, &entry, str_num, positive);
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
  return strlen(buff);
}

int main() {
  char buff[500] = {'\0'};
  // String
  int res = s21_sprintf(buff, "%7.7f", -11.1234560);
  printf("%d\n", res);
  printf("%s", buff);
  return 0;
}
