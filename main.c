#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

#define STRING_IMPL
#include "external/string.h"

#define PATH_IMPL
#include "external/path.h"

#define CURSOR_IMPL
#include "external/cursor.h"

#include "logo.h"

const int TOP_MARGIN = 2;
const int MODULES_MARGIN_FROM_LOGO = 3;

const int DAY_IN_SECONDS = 86400;
const int HOUR_IN_SECONDS = 3600;
const int MINUTE_IN_SECONDS = 60;

void printOS();
void printCpu();
void printBattery();
void printKernel();
void printShell();
void printUpTime();
void printPackages();
void printTerm();
void printDektop();

void (*const modules[])() = {
    // printOS,
    printCpu,
    // printBattery,
    // printKernel,
    // printDektop,
    // printShell,
    // printPackages,
    // printUpTime,
    // printTerm,
};

const int modulesSize = sizeof(modules) / sizeof(modules[0]);

typedef struct {
	strSlice data;
    int x;
    int y;
} Logo;

typedef struct {
    const char *name;
    const char *command;
} PackageQuery;

const PackageQuery packageQuery[] = {
    { "dpkg", "grep -sc \"^Package: \" /var/lib/dpkg/status" },
    { "pacman", "ls -f /var/lib/pacman/local 2>/dev/null | wc -l" },
    { "flatpak", "find /var/lib/flatpak/app /var/lib/flatpak/runtime -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l" },
    { "rpm", "dnf list installed 2>/dev/null | wc -l" },
    { "xbps", "xbps-query -l 2>/dev/null | wc -l"},
};

const int packageQuerySize = sizeof(packageQuery) / sizeof(PackageQuery);

int max(int a, int b)
{
    return a > b ? a : b;
}

void printOS()
{
	return;
    // string file = newStr("");
    // if (readWholeFileB(&file, "/etc/os-release", 4096) != Ok) {
    //     strFree(&file);
    //     return;
    // }
    // strView prefix = newStrView("PRETTY_NAME=\"");
    // int start = strStr(file, prefix, 0);
    // if (start == -1) {
    //     strFree(&file);
    //     return;
    // }
    // start += prefix.len;
    // int end = strStr(file, newStrView("\"\n"), start);
    // if (end == -1) {
    //     strFree(&file);
    //     return;
    // }
    // strView res = newStrViewRange(file, start, end);
    // printf(B1"│ OS:"C0" %.*s\n", res.len, res.data);

    // strFree(&file);
}

void printCpu()
{
    struct utsname info;
    if (uname(&info) == 0)
        printf(B2 "│ CPU:" C0 " %s\n", info.machine);
}

void printBattery()
{
    int capacity;
    if (readFile("/sys/class/power_supply/BAT0/capacity", "%d", &capacity) == Ok)
        printf(B3 "│ Battery:" C0 " %d%%\n", capacity);
}

void printKernel()
{
    struct utsname info;
    if (uname(&info) == 0)
        printf(B4 "│ Kernel:" C0 " %s\n", info.release);
}

void printShell()
{
    const char *shell = getenv("SHELL");
    if (shell != NULL)
        printf(B6 "│ Shell:" C0 " %s\n", shell);
}

void printUpTime()
{
    int seconds;
    if (readFile("/proc/uptime", "%d ", &seconds) != Ok) {
        return;
    }

	int days    = (seconds / DAY_IN_SECONDS);
	int hours   = (seconds / HOUR_IN_SECONDS) % 24;
	int minutes = (seconds / MINUTE_IN_SECONDS) % 60;

    printf(B8"│ Uptime:"C0 " ");

    if (days) {
        printf("%d days", days);
    }

    if (hours) {
        if (days)
            printf(", ");
        printf("%d hours", hours);
    }

    if (minutes) {
        if (hours || days)
            printf(", ");
        printf("%d mins", minutes);
    }

    printf("\n");
}

void printPackage(PackageQuery pq)
{
	FILE *fp = popen(pq.command, "r");
	if (fp == NULL)
		return;

	int num;
	if (fscanf(fp, "%d", &num) != 1) {
		pclose(fp);
		return;
	}

    printf(" %d (%s),", num, pq.name);
	pclose(fp);
}

void printPackages()
{
    printf(B7"│ Packages:"C0"");

    for (int i = 0; i < packageQuerySize; i++) {
        PackageQuery pq = packageQuery[i];
        printPackage(pq);
    }

    cursorLeft(1); // remove the last ','
    printf(" \n");
}

void printTerm()
{
    const char *term = getenv("TERM");
    if (term != NULL)
        printf(B1 "│ Term: " C0 "%s\n", term);
}

void printDektop()
{
    const char *desktop = getenv("DESKTOP_SESSION");
    if (desktop != NULL)
        printf(B5 "│ Desktop:" C0 " %s\n", desktop);
}

Logo newLogo(const char *data)
{
    Logo logo = {
		.data = newStrSlice(data, strlen(data)),
		.x = 0,
		.y = strCountc(&logo.data, '\n'),
	};

	strSlice line = strTokStart(&logo.data, "\n");
	while (!strIsEmpty(&line)) {
		logo.x = max(logo.x, strDisplayedLength(&line));
		line = strTok(&logo.data, &line, "\n");
	}

    return logo;
}

void printLogo(const Logo *logo)
{
    strPrint(&logo->data);
}

void printModules(void (*const modules[])(), int size, int rightOffset)
{
    for (int i = 0; i < size; i++) {
        setCursorX(rightOffset);
        modules[i]();
    }
}

int main(void)
{
    hideCursor();
    enableFullBuffering(stdout);
    disableLineWrap();

    Logo logo = newLogo(LOGO);
    printLogo(&logo);

    cursorUp(logo.y - TOP_MARGIN);

    printModules(modules, modulesSize, logo.x + MODULES_MARGIN_FROM_LOGO + 1);

    setCursorX(0);
    if (logo.y > modulesSize + TOP_MARGIN)
        cursorDown(logo.y - TOP_MARGIN - modulesSize + 1);

    enableLineWrap();
    showCursor();

    return EXIT_SUCCESS;
}
