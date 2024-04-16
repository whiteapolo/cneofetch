#ifndef FILES_UTILS_H
#define FILES_UTILS_H

#include <stdio.h>
#include <string.h>

int read_substring_line(const char *file_name, const char *sub, char *buff, unsigned int buff_len);
int read_line_from_file(const char *file_name, unsigned int line, char *buff, unsigned int buff_len);
int read_command(const char *command, char *buff, unsigned int buff_len);

int read_substring_line(const char *file_name, const char *sub, char *buff, unsigned int buff_len)
{
	FILE *file = fopen(file_name, "rb");
	if (!file)
	          return 1;

	while (fgets(buff, 50, file)) {
	          if (strstr(buff, sub)) {
	      		fclose(file);
	      		return 0;
	          }
	}
	fclose(file);
	return 1;
}

int read_line_from_file(const char *file_name, unsigned int line, char *buff, unsigned int buff_len)
{
	FILE *file = fopen(file_name, "rb");
	if (!file)
		return 1;
	while(fgets(buff, buff_len, file) && --line);
	fclose(file);
	return 0;
}

int read_command(const char *command, char *buff, unsigned int buff_len)
{
	FILE *fp = popen(command, "r");
	if (!fp || !fgets(buff, buff_len, fp))
		return 1;

	pclose(fp);
	return 0;
}

#endif
