#include "modules.h"

#define ASSERT(a) if(a) return;

#define DAY_IN_SECONDS 86400
#define HOUR_IN_SECONDS 3600
#define MINUTE_IN_SECONDS 60

void __format_vprint(int num, char *buff, const char *label_name, bool *comma);


void get_os(char *dest)
{
	char buffer[BUFFER_LEN];
	ASSERT(read_substring_line("/etc/os-release", "PRETTY_NAME", buffer, BUFFER_LEN));
	char *start = strchr(buffer, '"');
	char *end = strrchr(buffer, '"');
	ASSERT(!end || !start);
	*end = '\0';
	sprintf(dest, OS_LABEL, ++start);
}

void get_cpu(char *dest)
{
	char buffer[BUFFER_LEN];
	ASSERT(read_line_from_file("/proc/cpuinfo", 5, buffer, BUFFER_LEN));
	char *start = strchr(buffer, ':');
	ASSERT(!start);
	char *end = strnchr(start + 2, ' ', CPU_MODELWORDS);
	ASSERT(!end);
	*end = '\0';
	sprintf(dest,CPU_LABEL, start + 2);
}


void get_battery(char *dest)
{
	char buffer[BUFFER_LEN];
	ASSERT(read_line_from_file("/sys/class/power_supply/BAT0/capacity", 1, buffer, BUFFER_LEN));
	replace_char_in_string(buffer, '\n', '%');
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
	str_lower(dest);
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
		__format_vprint(pkg_n, end, package_query[i][0], &comma);
		end += strlen(end);
	}
}

void get_uptime(char *dest)
{
	char buffer[BUFFER_LEN];
	ASSERT(read_line_from_file("/proc/uptime", 1, buffer, BUFFER_LEN));
	char *dividor = strchr(buffer, ' ');
	ASSERT(!dividor++);

	dividor[strlen(dividor) - 2] = '\0';

	int sec = atoi(buffer);

	int d = sec/ (DAY_IN_SECONDS);
	int h = (sec/ HOUR_IN_SECONDS) % 24;
	int m = (sec/ MINUTE_IN_SECONDS) % 60;

	strcpy(dest, UPTIME_LABEL);
	char *end = &dest[strlen(dest)];

	bool comma = false;

	__format_vprint(d, end, "days", &comma);
	end += strlen(end);
	__format_vprint(h, end, "hours", &comma);
	end += strlen(end);
	__format_vprint(m, end, "mins", &comma);
	end += strlen(end);
}


/* local */


void __format_vprint(int num, char *buff, const char *label_name, bool *comma)
{
	if (num) {
		if (*comma)
			strcat(buff++, ",");
		sprintf(buff, " %d %s", num , label_name);
		*comma = true;
	}
}
