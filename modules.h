#ifndef MODULES_H
#define MODULES_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <stdio.h>

#define PACKAGES_QUERY "grep -c \"^Package: \" /var/lib/dpkg/status"
#define BUFFER_LEN 70
#define CPU_MODELWORDS 3

#define OS_LABEL	"\e[1;91m│\e[0m \x1b[1;31mOS:\x1b[0m %s"
#define CPU_LABEL	"\e[1;92m│\e[0m \x1b[1;32mCPU:\x1b[0m %s"
#define BATTERY_LABEL	"\e[1;93m│\e[0m \x1b[1;33mBattery:\x1b[0m %s"
#define KERNEL_LABEL	"\e[1;94m│\e[0m \x1b[1;34mKernel:\x1b[0m %s"
#define DESKTOP_LABEL	"\e[1;95m│\e[0m \x1b[1;35mdesktop:\x1b[0m %s"
#define SHELL_LABEL	"\e[1;96m│\e[0m \x1b[1;36mshell:\x1b[0m %s"
#define PACKAGES_LABEL  "\e[1;97m│\e[0m \x1b[1;37mPackages:\x1b[0m %s"
#define UPTIME_LABEL    "\e[1;90m│\e[0m \x1b[1;30mUptime:\x1b[0m"

#define DEBIAN 0
#define ARCH 2
#define VOID 3

#define ASSERT(a) if(a) return;

static char buffer[BUFFER_LEN];

/**********************************************************************
 *                        DECLARATIONS                                *
 *********************************************************************/

void get_os(char *dest);
void get_cpu(char *dest);
void get_battery(char *dest);
void get_kernel(char *dest);
void get_desktop(char *dest);
void get_shell(char *dest);
void get_uptime(char *dest);
void get_packages(char *dest);

char *_strnchr(char *str, char c, int n);
void _str_lower(char *str);
void _replace_char_in_string(char *str, char from, char to);
int _read_line_from_file(const char *fileName, unsigned short line, char *buffer, unsigned char bufferLength);
int _read_substring_line(const char *fileName, const char *sub, char *buffer, unsigned char bufferLength);

char *strcasestr(const char *haystack, const char *needle);

int ___count_pattern_match(const char *fileName, char *pattern);


/**********************************************************************
 *                       IMPLEMENTATION                               *
 *********************************************************************/

void get_os(char *dest)
{
	ASSERT(_read_substring_line("/etc/os-release", "PRETTY_NAME", buffer, BUFFER_LEN));
	char *start = strchr(buffer, '"');
	char *end = strrchr(buffer, '"');
	ASSERT(!end || !start);
	*end = '\0';
	sprintf(dest, OS_LABEL, ++start);
}

void get_cpu(char *dest)
{
	ASSERT(_read_line_from_file("/proc/cpuinfo", 5, buffer, BUFFER_LEN));
	char *start = strchr(buffer, ':');
	char *end = _strnchr(start + 2, ' ', CPU_MODELWORDS);
	ASSERT(!start || !end);
	*end = '\0';
	sprintf(dest,CPU_LABEL, start + 2);
}


void get_battery(char *dest)
{
	ASSERT(_read_line_from_file("/sys/class/power_supply/BAT0/capacity", 1, buffer, BUFFER_LEN));
	_replace_char_in_string(buffer, '\n', '%');
	sprintf(dest, BATTERY_LABEL, buffer);
}

void get_kernel(char *dest)
{
	struct utsname info;
	ASSERT(uname(&info));
	sprintf(dest, KERNEL_LABEL, info.release);
}

void get_desktop(char *dest)
{
	const char *desktop = getenv("XDG_CURRENT_DESKTOP");
	ASSERT(!desktop);
	sprintf(dest, DESKTOP_LABEL, desktop);
	_str_lower(dest);
}

void get_shell(char *dest)
{
	const char *shell = getenv("SHELL");
	ASSERT(!shell);
	sprintf(dest, SHELL_LABEL, shell);
}

void get_packages(char *dest)
{
	FILE *fp = popen(PACKAGES_QUERY, "r");
	ASSERT(!fp);
	fgets(buffer, BUFFER_LEN, fp);
	pclose(fp);
	char *tmp = strchr(buffer, '\n');
	if(tmp)
		*tmp = '\0';
	sprintf(dest, PACKAGES_LABEL, buffer);
}

void get_uptime(char *dest)
{
	ASSERT(_read_line_from_file("/proc/uptime", 1, buffer, BUFFER_LEN));
	char *dividor = strchr(buffer, ' ');
	ASSERT(!dividor++);

	dividor[strlen(dividor) - 2] = '\0';

	int seconds = atoi(buffer);

	int days = seconds / (60 * 60 * 24);
	int hours = (seconds / 3600) % 24;
	int mins = (seconds / 60) % 60;

	strcpy(dest, UPTIME_LABEL);
	char *tmp = &dest[strlen(dest)];

	if (days)
		sprintf(tmp, " %d days,", days);

	tmp += strlen(tmp);
	if (hours)
		sprintf(tmp, " %d hours,", hours);

	tmp += strlen(tmp);
	if (mins)
		sprintf(tmp, " %d mins", mins);
}

/**********************************************************************
 *                       LOCAL FUNCTIONS                              *
 *********************************************************************/

char *_strnchr(char *str, char c, int n)
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

void _str_lower(char *str) {
	  while((*(str++) = tolower(*str)));
}

void _replace_char_in_string(char *str, char from, char to) {
	 char *tmp = strchr(buffer, from);
	 if (tmp)
		*tmp = to;
}

int _read_substring_line(const char *fileName, const char *sub, char *buffer, unsigned char bufferLength)
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

int _read_line_from_file(const char *fileName, unsigned short line, char *buffer, unsigned char bufferLength)
{
	FILE *file = fopen(fileName, "rb");
	if (!file)
		return 1;
	while(fgets(buffer, bufferLength, file) && --line);
	fclose(file);
	return 0;
}

#endif
