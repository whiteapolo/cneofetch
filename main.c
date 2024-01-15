#include <stdio.h>
#include <string.h>
#include "modules.h"

#define MODULES_COUNT 8

void remove_empty_modules(char offsets[MODULES_COUNT], char modules[MODULES_COUNT][BUFFER_LEN])
{
	char tmp;
	int j = 0;
	for (int i = 0; i < MODULES_COUNT; i++) {
		if (modules[i][0])
			offsets[j++] = i;
		else 
			tmp = i;
	}
	while (j < MODULES_COUNT)
		offsets[j++] = tmp;
}

void main(void)
{
	char modules[MODULES_COUNT][BUFFER_LEN] = {0};
	char offsets[MODULES_COUNT];

	get_os(modules[0]);
	get_cpu(modules[1]);
	get_battery(modules[2]);
	get_kernel(modules[3]);
	get_desktop(modules[4]);
	get_shell(modules[5]);
	get_packages(modules[6]);
	get_uptime(modules[7]);

	remove_empty_modules(offsets, modules);

	printf(
	"         \x1b[90m#####\n"
	"        #######\n"
	"        ##\e[0m0\x1b[90m#\e[0m0\x1b[90m##             %s\n"
	"        \x1b[90m#\x1b[33m#####\x1b[90m#             %s\n"
	"      \x1b[90m##\e[0m##\x1b[33m###\e[0m##\x1b[90m##           %s\n"
	"     \x1b[90m#\e[0m##########\x1b[90m##          %s\n"
	"    \x1b[90m#\e[0m############\x1b[90m##         %s\n"
	"    \x1b[90m#\e[0m###########\x1b[90m####        %s\n"
	"   \x1b[33m##\x1b[90m#\e[0m###########\x1b[90m##\x1b[33m#        %s\n"
	" \x1b[33m######\x1b[90m#\e[0m#######\x1b[90m#\x1b[33m######      %s\n"
	" \x1b[33m#######\x1b[90m#\e[0m#####\x1b[90m#\x1b[33m#######\n"
	"   #####\x1b[90m#######\x1b[33m#####\n\e[0m",
	modules[offsets[0]], 
	modules[offsets[1]], 
	modules[offsets[2]], 
	modules[offsets[3]], 
	modules[offsets[4]], 
	modules[offsets[5]], 
	modules[offsets[6]], 
	modules[offsets[7]]);
}
