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
#include "utils/files_utils.h"
#include "utils/string_utils.h"

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



void get_os(char *dest);
void get_cpu(char *dest);
void get_battery(char *dest);
void get_kernel(char *dest);
void get_desktop(char *dest);
void get_shell(char *dest);
void get_uptime(char *dest);
void get_packages(char *dest);
void get_term(char *dest);

#endif
