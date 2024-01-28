#ifndef FILES_UTILS_H
#define FILES_UTILS_H

#include <stdio.h>
#include <string.h>

int read_substring_line(const char *file_name, const char *sub, char *buff, unsigned int buff_len);
int read_line_from_file(const char *file_name, unsigned int line, char *buff, unsigned int buff_len);
int read_command(const char *command, char *buff, unsigned int buff_len);

#endif
