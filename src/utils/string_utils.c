#include "string_utils.h"

char *strnchr(char *str, char c, int n)
{
	char *result = str;
	for (int i = 0; i < n; i++) {
		result = strchr(result, c);
		if (!result)
			return NULL;
		result++;
	}
	return --result;
}

void str_lower(char *str) 
{
	  while((*(str++) = tolower(*str)));
}

void replace_char_in_string(char *str, char from, char to) 
{
	 char *tmp = strchr(str, from);
	 if (tmp)
		*tmp = to;
}
