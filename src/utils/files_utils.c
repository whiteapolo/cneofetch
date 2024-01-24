#include "files_utils.h"

int read_substring_line(const char *fileName, const char *sub, char *buffer, unsigned int bufferLength)
{
	FILE *file = fopen(fileName, "rb");
	if (!file)
	          return 1;

	while (fgets(buffer, 50, file)) {
	          if (strstr(buffer, sub)) {
	      		fclose(file);
	      		return 0;
	          }
	}
	fclose(file);
	return 1;
}

int read_line_from_file(const char *fileName, unsigned int line, char *buffer, unsigned int bufferLength)
{
	FILE *file = fopen(fileName, "rb");
	if (!file)
		return 1;
	while(fgets(buffer, bufferLength, file) && --line);
	fclose(file);
	return 0;
}
