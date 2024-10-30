#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

#define STRING_IMPL
#include "mystb/string.h"

#define PATH_IMPL
#include "mystb/path.h"

#define CURSOR_IMPL
#include "mystb/cursor.h"

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
    printOS,
    printCpu,
    printBattery,
    printKernel,
    printDektop,
    printShell,
    printPackages,
    printUpTime,
    // printTerm,
};

const unsigned int modulesSize = sizeof(modules) / sizeof(modules[0]);

typedef struct {
	strSlice data;
    unsigned int x;
    unsigned int y;
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
	string s = readWholeFile("/etc/os-release");
	if (strIsEmpty(s))
		return;
	strSlice name = strExtractBetweenC(s, "PRETTY_NAME=\"", "\"");
	if (!strIsEmpty(name))
		printf(B1"│ OS:"C0" %.*s\n", (int)name.len, name.data);
	strFree(&s);
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
	unsigned int seconds;
	if (readFile("/proc/uptime", "%u ", &seconds) != Ok)
		return;

	const unsigned int days = (seconds / DAY_IN_SECONDS);
	const unsigned int hours = (seconds / HOUR_IN_SECONDS) % 24;
	const unsigned int minutes = (seconds / MINUTE_IN_SECONDS) % 60;

    printf(B8"│ Uptime:"C0 " ");
	if (days > 0)
		printf("%d days, ", days);
	if (hours > 0)
		printf("%d hours, ", hours);
	if (minutes > 0)
		printf("%d mins, ", minutes);
	cursorLeft(2);
    printf("  \n");
}

void printPackage(PackageQuery pq)
{
	FILE *fp = popen(pq.command, "r");
	if (fp == NULL)
		return;

	int num;
	if (fscanf(fp, "%d", &num) != 1 || num == 0) {
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
    Logo logo;
	logo.data = sliceStr(data, strlen(data));
	logo.x = 0;
	logo.y = strCountc(logo.data, '\n');

	strSlice line = strTokStart(logo.data, "\n");
	while (!strIsEmpty(line)) {
		logo.x = max(logo.x, strDisplayedLength(line));
		line = strTok(logo.data, line, "\n");
	}

    return logo;
}

void printLogo(const Logo *logo)
{
    strPrint(logo->data);
}

void printModules(const int rightOffset)
{
    for (unsigned short i = 0; i < modulesSize; i++) {
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

    printModules(logo.x + MODULES_MARGIN_FROM_LOGO + 1);

    setCursorX(0);
    if (logo.y > modulesSize + TOP_MARGIN)
        cursorDown(logo.y - TOP_MARGIN - modulesSize);

    enableLineWrap();
    showCursor();

    return EXIT_SUCCESS;
}
