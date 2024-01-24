#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "config.h"
#include "modules.h"
#include "utils/cursors_utils.h"

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

#define MARGIN_BOTTOM 10

int main(void)
{
	char buffer[BUFFER_LEN];
	buffer[0] = '\0';

	fputs(LOGO, stdout);
	fflush(stdout);

	CURSOR_HIDE();

	for (int i = 0; i < MODULES_COUNT; i++) {
		SAVE_CURSOR();
		CURSOR_UP(MARGIN_BOTTOM - i);
		CURSOR_RIGHT(26);
		modules[i](buffer);
		fputs(buffer, stdout);
		fflush(stdout);
		i -= !buffer[0];
		RESTORE_CURSOR();
	}

	CURSOR_SHOW();

	return 0;
}
