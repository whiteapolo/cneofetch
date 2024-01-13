#include <stdio.h>
#include "modules.h"

#define MODULES_COUNT 8

void main(void)
{
	  char modules[MODULES_COUNT][BUFFER_LEN];
	  get_os(modules[0]);
	  get_cpu(modules[1]);
	  get_battery(modules[2]);
	  get_kernel(modules[3]);
	  get_desktop(modules[4]);
	  get_shell(modules[5]);
	  get_packages(modules[6]);
	  get_uptime(modules[7]);

	  printf(
	  "         \x1b[90m#####\n"
	  "        #######\n"
	  "        ##\e[0m0\x1b[90m#\e[0m0\x1b[90m##             \e[1;91m│\e[0m %s\n"
	  "        \x1b[90m#\x1b[33m#####\x1b[90m#             \e[1;92m│\e[0m %s\n"
	  "      \x1b[90m##\e[0m##\x1b[33m###\e[0m##\x1b[90m##           \e[1;93m│\e[0m %s\n"
	  "     \x1b[90m#\e[0m##########\x1b[90m##          \e[1;94m│\e[0m %s\n"
	  "    \x1b[90m#\e[0m############\x1b[90m##         \e[1;95m│\e[0m %s\n"
	  "    \x1b[90m#\e[0m###########\x1b[90m####        \e[1;96m│\e[0m %s\n"
	  "   \x1b[33m##\x1b[90m#\e[0m###########\x1b[90m##\x1b[33m#        \e[1;97m│\e[0m %s\n"
	  " \x1b[33m######\x1b[90m#\e[0m#######\x1b[90m#\x1b[33m######      \e[1;90m│\e[0m %s\n"
	  " \x1b[33m#######\x1b[90m#\e[0m#####\x1b[90m#\x1b[33m#######\n"
	  "   #####\x1b[90m#######\x1b[33m#####\n\e[0m", modules[0], modules[1], modules[2], modules[3], modules[4], modules[5], modules[6], modules[7]);
}
