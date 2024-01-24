#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string.h>
#include <ctype.h>

char *strnchr(char *str, char c, int n);
void str_lower(char *str);
void replace_char_in_string(char *str, char from, char to);

#endif
