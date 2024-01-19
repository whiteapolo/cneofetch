#ifndef MODULES_H
#define MODULES_H


/*----------TODO-----------*/
/*
 * TERM 
 * df -h /
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <stdbool.h>
#include <stdio.h>

#define BUFFER_LEN 70
#define CPU_MODELWORDS 3

#define OS_LABEL       "\e[1;91m│\e[0m \x1b[1;31mOS:\x1b[0m %s"
#define CPU_LABEL      "\e[1;92m│\e[0m \x1b[1;32mCPU:\x1b[0m %s"
#define BATTERY_LABEL  "\e[1;93m│\e[0m \x1b[1;33mBattery:\x1b[0m %s"
#define KERNEL_LABEL   "\e[1;94m│\e[0m \x1b[1;34mKernel:\x1b[0m %s"
#define DESKTOP_LABEL  "\e[1;95m│\e[0m \x1b[1;35mdesktop:\x1b[0m %s"
#define SHELL_LABEL    "\e[1;96m│\e[0m \x1b[1;36mshell:\x1b[0m %s"
#define PACKAGES_LABEL "\e[1;97m│\e[0m \x1b[1;37mPackages:\x1b[0m"
#define UPTIME_LABEL   "\e[1;90m│\e[0m \x1b[1;30mUptime:\x1b[0m"

#define DAY_IN_SECONDS 86400
#define HOUR_IN_SECONDS 3600
#define MINUTE_IN_SECONDS 60

#define ASSERT(a) if(a) return;

#define __VPRINT(num, buff, label_name, comma, parentasis)            \
	if (num) {                                                    \
		if (comma)                                            \
			strcat(buff++, ",");                          \
		if (parentasis)                                       \
			sprintf(buff, " %d (%s)", num, label_name);   \
		else                                                  \
			sprintf(buff, " %d %s", num, label_name);     \
		comma = true;                                         \
	}                                                             \


static const char package_query[][2][256] = {
{"dpkg", "grep -c \"^Package: \" /var/lib/dpkg/status 2>/dev/null"},
{"pacman", "ls /var/lib/pacman/local 2>/dev/null | wc -l"},
{"flatpak", "find /var/lib/flatpak/app /var/lib/flatpak/runtime -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l"},
{"xbps", "xbps-query -l 2>/dev/null | wc -l"}
};


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
int _read_line_from_file(const char *fileName, unsigned int line, char *buffer, unsigned int bufferLength);
int _read_substring_line(const char *fileName, const char *sub, char *buffer, unsigned int bufferLength);

char *strcasestr(const char *haystack, const char *needle);


/**********************************************************************
 *                       IMPLEMENTATION                               *
 *********************************************************************/

void get_os(char *dest)
{
	char buffer[BUFFER_LEN];
	ASSERT(_read_substring_line("/etc/os-release", "PRETTY_NAME", buffer, BUFFER_LEN));
	char *start = strchr(buffer, '"');
	char *end = strrchr(buffer, '"');
	ASSERT(!end || !start);
	*end = '\0';
	sprintf(dest, OS_LABEL, ++start);
}

void get_cpu(char *dest)
{
	char buffer[BUFFER_LEN];
	ASSERT(_read_line_from_file("/proc/cpuinfo", 5, buffer, BUFFER_LEN));
	char *start = strchr(buffer, ':');
	ASSERT(!start);
	char *end = _strnchr(start + 2, ' ', CPU_MODELWORDS);
	ASSERT(!end);
	*end = '\0';
	sprintf(dest,CPU_LABEL, start + 2);
}


void get_battery(char *dest)
{
	char buffer[BUFFER_LEN];
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

int __get_package(const char *command)
{
	FILE *fp = popen(command, "r");
	if (!fp) {
		return 0;
	}

	char buffer[BUFFER_LEN];
	fgets(buffer, BUFFER_LEN, fp);
	pclose(fp);

	return atoi(buffer);
}

void get_packages(char *dest)
{
	strcpy(dest, PACKAGES_LABEL);
	char *end = &dest[strlen(dest)];
	bool comma = false;
	int pkg_n = 0;

	for (int i = 0; i < sizeof(package_query)/sizeof(package_query[0]); i++) {
		pkg_n = __get_package(package_query[i][1]);
		__VPRINT(pkg_n, end, package_query[i][0], comma, true);
		end += strlen(end);
	}
}

void get_uptime(char *dest)
{
	char buffer[BUFFER_LEN];
	ASSERT(_read_line_from_file("/proc/uptime", 1, buffer, BUFFER_LEN));
	char *dividor = strchr(buffer, ' ');
	ASSERT(!dividor++);

	dividor[strlen(dividor) - 2] = '\0';

	int seconds = atoi(buffer);

	int d = seconds / (DAY_IN_SECONDS);
	int h = (seconds / HOUR_IN_SECONDS) % 24;
	int m = (seconds / MINUTE_IN_SECONDS) % 60;

	strcpy(dest, UPTIME_LABEL);
	char *end = &dest[strlen(dest)];

	bool comma = false;

	__VPRINT(d, end, "days", comma, false);
	end += strlen(end);
	__VPRINT(h, end, "hours", comma, false);
	end += strlen(end);
	__VPRINT(m, end, "mins", comma, false);
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
	 char *tmp = strchr(str, from);
	 if (tmp)
		*tmp = to;
}

int _read_substring_line(const char *fileName, const char *sub, char *buffer, unsigned int bufferLength)
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

int _read_line_from_file(const char *fileName, unsigned int line, char *buffer, unsigned int bufferLength)
{
	FILE *file = fopen(fileName, "rb");
	if (!file)
		return 1;
	while(fgets(buffer, bufferLength, file) && --line);
	fclose(file);
	return 0;
}

#endif
