#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "config.h"
#include "modules.h"
#include <ncurses.h>
#include "utils/cursors_utils.h"

void (*modules[])(char *dest) = {
	    get_os,
	    get_cpu,
	    get_battery,
	    get_kernel,
	    get_shell,
	    get_packages,
	    get_uptime
};

#define MODULES_COUNT (sizeof(modules)/sizeof(modules[0]))

#define MARGIN_BOTTOM 10

int main(void)
{
	char buffer[BUFF_LEN];

	fputs(LOGO, stdout);
	fflush(stdout);

	CURSOR_HIDE();

	for (int i = 0; i < MODULES_COUNT; i++) {
		SAVE_CURSOR();
		CURSOR_UP(MARGIN_BOTTOM - i);
		CURSOR_RIGHT(26);
		buffer[0] = '\0';
		modules[i](buffer);
		fputs(buffer, stdout);
		fflush(stdout);
		i -= !buffer[0];
		RESTORE_CURSOR();
	}

	CURSOR_SHOW();

	return 0;
}
