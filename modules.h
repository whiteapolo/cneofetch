#ifndef MODULES_H
#define MODULES_H

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <stdio.h>

#define PACKAGE_QUERY "dpkg-query -f '${binary:Package}\n' -W | wc -l"
#define BUFFER_LEN 40
#define ASSERT(a) do { if(a) exit(1); } while(0)

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

void str_lower(char *str);
void replace_char_in_string(char *str, char from, char to);
int read_line_from_file(const char fileName[], unsigned short line, char *buffer, unsigned char bufferLength);
int read_substring_line(const char fileName[], const char *sub, char *buffer, unsigned char bufferLength);


/**********************************************************************
 *                       IMPLEMENTATION                               *
 *********************************************************************/

void get_os(char *dest)
{
	  strcpy(dest, "\x1b[1;31mOS:\x1b[0m ");
	  if (read_substring_line("/etc/os-release", "PRETTY_NAME", buffer, BUFFER_LEN))
		    return;

	  char *start = strchr(buffer, '"');
	  char *end = strrchr(buffer, '"');
	  if (!end || !start) 
		    return;

	  *end = '\0';
	  strcat(dest, ++start);
}

void get_cpu(char *dest)
{
	  if (read_line_from_file("/proc/cpuinfo", 5, buffer, BUFFER_LEN))
		    return;

	  char *start = strchr(buffer, ':');

	  if (!start)
		    return;

	  strcpy(dest, "\x1b[1;32mCPU:\x1b[0m ");

	  (start++)[20] = '\0';
	  strcat(dest, ++start);
}

void get_battery(char *dest)
{
	  strcpy(dest, "\x1b[1;33mBattery:\x1b[0m ");
	  if (read_line_from_file("/sys/class/power_supply/BAT0/capacity", 1, buffer, BUFFER_LEN)) 
		    return;
	  replace_char_in_string(buffer, '\n', '%');
	  strcat(dest, buffer);
}

void get_kernel(char *dest)
{
	  struct utsname info;
	  if (uname(&info))
		    return;

	  strcpy(dest, "\x1b[1;34mKernel: \x1b[0m");
	  strcat(dest, info.release);
}

void get_desktop(char *dest)
{
	  const char *desktop = getenv("XDG_CURRENT_DESKTOP");
	  if (!desktop)
		    return;
	  strcpy(dest, "\x1b[1;35mDesktop: \x1b[0m");
	  strcat(dest, desktop);

	  str_lower(dest);
}


void get_shell(char *dest)
{
	  const char *shell = getenv("SHELL");

	  if(!shell)
		    return;

	  strcpy(dest, "\x1b[1;36mShell: \x1b[0m");
	  strcat(dest, shell);
}


void get_packages(char *dest)
{
	FILE *fp;
	fp = popen(PACKAGE_QUERY, "r");
	if (!fp) 
	    return;

	strcpy(dest, "\x1b[1;37mPackages:\x1b[0m ");

	fgets(buffer, 8, fp);
	pclose(fp);
	char *tmp = strchr(buffer, '\n');
	if(tmp)
		*tmp = '\0';
	strcat(dest, buffer);
}

void get_uptime(char *dest)
{
	ASSERT(read_line_from_file("/proc/uptime", 1, buffer, BUFFER_LEN));

	char *dividor = strchr(buffer, ' ');
	ASSERT(!dividor++);

	dividor[strlen(dividor) - 2] = '\0';

	int seconds = atoi(buffer);

	int days = seconds / (60 * 60 * 24);
	int hours = (seconds / 3600) % 24;
	int mins = (seconds / 60) % 60;

	strcpy(dest, "\x1b[1;30mUptime:\x1b[0m");
	char *tmp = dest;

	tmp += strlen(tmp);
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

void str_lower(char *str) {
	  while((*(str++) = tolower(*str)));
}

void replace_char_in_string(char *str, char from, char to) {
	  char *tmp = strchr(buffer, from);
	  if (tmp)
		    *tmp = to;
}

int read_substring_line(const char fileName[], const char *sub, char *buffer, unsigned char bufferLength)
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

int read_line_from_file(const char fileName[], unsigned short line, char *buffer, unsigned char bufferLength)
{
	  FILE *file = fopen(fileName, "rb");
	  if (!file)
		    return 1;
	  while(fgets(buffer, bufferLength, file) && --line);
	  fclose(file);
	  return 0;
}

#endif
