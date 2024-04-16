#ifndef MODULES_H
#define MODULES_H

/*----------TODO-----------*/
/*
 * df -h /
 */

#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <stdbool.h>
#include <stdio.h>
#include "config.h"
#include "files_utils.h"
#include "string_utils.h"

#define BUFF_LEN 70

#define OS_LABEL          B1"│ OS:"C0" %s"
#define CPU_LABEL         B2"│ CPU:"C0" %s"
#define BATTERY_LABEL     B3"│ Battery:"C0" %s"
#define KERNEL_LABEL      B4"│ Kernel:"C0" %s"
#define DESKTOP_LABEL     B5"│ desktop:"C0" %s"
#define SHELL_LABEL       B6"│ shell:"C0" %s"
#define PACKAGES_LABEL    B7"│ Packages:"C0
#define UPTIME_LABEL      B8"│ Uptime:"C0
#define TERM_LABEL        B1"│ term:"C0" %s"


bool get_os(char *dest);
bool get_cpu(char *dest);
bool get_battery(char *dest);
bool get_kernel(char *dest);
bool get_desktop(char *dest);
bool get_shell(char *dest);
bool get_uptime(char *dest);
bool get_packages(char *dest);
bool get_term(char *dest);


#define ASSERT(a) if(a) return false;

#define DAY_IN_SECONDS 86400
#define HOUR_IN_SECONDS 3600
#define MINUTE_IN_SECONDS 60

void __format_vprint(int num, char *buff, const char *label_name, bool *comma);

bool get_os(char *dest)
{
	char buff[BUFF_LEN];
	ASSERT(read_substring_line("/etc/os-release", "PRETTY_NAME", buff, BUFF_LEN));
	char *start = strchr(buff, '"');
	char *end = strrchr(buff, '"');
	ASSERT(!end || !start);
	*end = '\0';
	sprintf(dest, OS_LABEL, ++start);
	return true;
}

bool get_cpu(char *dest)
{
	char buff[BUFF_LEN];
	ASSERT(read_line_from_file("/proc/cpuinfo", 5, buff, BUFF_LEN));
	char *start = strchr(buff, ':');
	ASSERT(!start);
	char *end = strnchr(start + 2, ' ', CPU_MODELWORDS);
	ASSERT(!end);
	*end = '\0';
	sprintf(dest,CPU_LABEL, start + 2);
	/* struct utsname info; */
	/* ASSERT(uname(&info)); */
	/* sprintf(dest, CPU_LABEL, info.machine); */
	return true;
}


bool get_battery(char *dest)
{
	char buff[BUFF_LEN];
	ASSERT(read_line_from_file("/sys/class/power_supply/BAT0/capacity", 1, buff, BUFF_LEN));
	replace_char_in_string(buff, '\n', '%');
	sprintf(dest, BATTERY_LABEL, buff);
	return true;
}

bool get_kernel(char *dest)
{
	struct utsname info;
	ASSERT(uname(&info));
	sprintf(dest, KERNEL_LABEL, info.release);
	return true;
}

bool get_desktop(char *dest)
{
	const char *desktop = getenv("XDG_CURRENT_DESKTOP");
	ASSERT(!desktop);
	sprintf(dest, DESKTOP_LABEL, desktop);
	str_lower(dest);
	return true;
}

bool get_shell(char *dest)
{
	const char *shell = getenv("SHELL");
	ASSERT(!shell);
	sprintf(dest, SHELL_LABEL, shell);
	return true;
}

bool get_packages(char *dest)
{
	char buff[BUFF_LEN];
	char *end;
	bool comma = false;
	int pkg_n = 0;

	strcpy(dest, PACKAGES_LABEL);
	end = &dest[strlen(dest)];

	for (int i = 0; i < COMMAND_COUNT; i++) {
		pkg_n = read_command(package_query[i][1], buff, BUFF_LEN) 
			? 0 : atoi(buff);
		__format_vprint(pkg_n, end, package_query[i][0], &comma);
		end += strlen(end);
	}
	return true;
}

bool get_uptime(char *dest)
{
	char buff[BUFF_LEN];
	ASSERT(read_line_from_file("/proc/uptime", 1, buff, BUFF_LEN));
	char *dividor = strchr(buff, ' ');
	ASSERT(!dividor++);

	/* remove the floting point */
	dividor[strlen(dividor) - 2] = '\0';

	int sec = atoi(buff);

	int d = (sec / DAY_IN_SECONDS);
	int h = (sec / HOUR_IN_SECONDS) % 24;
	int m = (sec / MINUTE_IN_SECONDS) % 60;

	strcpy(dest, UPTIME_LABEL);
	char *end = &dest[strlen(dest)];

	bool comma = false;

	/* if minutes is zero than sets it to one */
	m += !m;

	__format_vprint(d, end, "days", &comma);
	end += strlen(end);
	__format_vprint(h, end, "hours", &comma);
	end += strlen(end);
	__format_vprint(m, end, "mins", &comma);
	end += strlen(end);
	return true;
}

bool get_term(char *dest)
{
	const char *term = getenv("TERM");
	ASSERT(!term);
	sprintf(dest, TERM_LABEL, term);
	return true;
}


void __format_vprint(int num, char *buff, const char *label_name, bool *comma)
{
	if (num) {
		if (*comma)
			strcat(buff++, ",");
		sprintf(buff, " %d %s", num, label_name);
		*comma = true;
	}
}

#endif
