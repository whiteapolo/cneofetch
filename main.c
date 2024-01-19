#include <stdio.h>
#include <string.h>
#include <time.h>
#include "logo.h"
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

#define MAX_MODULES_COUNT 8

void main(void)
{
	char results[MAX_MODULES_COUNT][BUFFER_LEN];

	for (int i = 0, j = 0; i < MODULES_COUNT; i++) {
		results[j][0] = '\0';
		modules[i](results[j]);
		j += (results[j][0] != '\0');
	}


	printf(LOGO,
		results[0],
		results[1],
		results[2],
		results[3],
		results[4],
		results[5],
		results[6],
		results[7]);
}
