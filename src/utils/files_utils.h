#ifndef FILES_UTILS_H
#define FILES_UTILS_H

#include <stdio.h>
#include <string.h>

int read_substring_line(const char *fileName, const char *sub, char *buffer, unsigned int bufferLength);

int read_line_from_file(const char *fileName, unsigned int line, char *buffer, unsigned int bufferLength);

#endif
