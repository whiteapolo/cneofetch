#include <stdio.h>
#include <string.h>
#include <time.h>
#include "config.h"
#include "modules.h"

void (*modules[])(char *dest) = {
	    get_os,
	    get_cpu,
	    get_battery,
	    get_kernel,
	    get_desktop,
	    get_shell,
	    get_packages,
	    get_uptime
};

#define MODULES_COUNT (sizeof(modules)/sizeof(modules[0]))

#define SAVE_CURSOR()           fputs("\033[s", stdout)
#define RESTORE_CURSOR()        fputs("\033[u", stdout)
#define CURSOR_DOWN(rows)       printf("\033[%dB", rows)
#define CURSOR_UP(rows)         printf("\033[%dA", rows)
#define CURSOR_RIGHT(columns)   printf("\033[%dC", columns)
#define CURSOR_LEFT(columns)    printf("\033[%dD", columns)

int main()
{
	char results[BUFFER_LEN];
	results[0] = '\0';

	fputs(LOGO, stdout);

	for (int i = 0; i < MODULES_COUNT; i++) {
		SAVE_CURSOR();
		CURSOR_UP(10 - i);
		CURSOR_RIGHT(26);
		modules[i](results);
		fputs(results, stdout);
		i -= !results[0];
		RESTORE_CURSOR();
	}

	return 0;
}
