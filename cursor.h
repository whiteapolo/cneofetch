#ifndef CURSOR_H
#define CURSOR_H

#include "error.h"

#define CTRL_KEY(k) ((k) & 0x1f)

enum KEY {
	ARROW_LEFT = 1000,
	ARROW_RIGHT = 1001,
	ARROW_UP = 1002,
	ARROW_DOWN = 1003,

	PAGE_UP = 1007,
	PAGE_DOWN = 1008,

	DELETE = 1004,
	HOME = 1005,
	END = 1006,
};

enum CURSOR_STYLE {
	BLOCK_STEADY = 0,
	BLOCK_BLINKING = 1,

	UNDERLINE_BLINKING = 3,
	UNDERLINE_STEADY = 4,

	BEAM_STEADY = 6,
	BEAM_BLINKING = 5,
};

ERROR enableRawMode();
ERROR disableRawMode();

void disableLineWrap();
void enableLineWrap();

void hideCursor();
void showCursor();

void setCursorStyle(enum CURSOR_STYLE style);

ERROR getCursorPos(int *x, int *y);
void setCursorPos(int x, int y);

void setCursorX(int x);

void cursorUp(int n);
void cursorDown(int n);
void cursorRight(int n);
void cursorLeft(int n);

// unsupoerted on some terminals
void saveCursorPos();
void restoreCursorPos();

void enterAlternativeScreen();
void exitAlternativeScreen();

void clearLine();
void clearScreen();

ERROR getScreenSizeByCursor(int *width, int *height);
ERROR getScreenSizeByIoctl(int *width, int *height);
ERROR getScreenSize(int *width, int *height);

ERROR registerChangeInWindowSize(void (*funciton)(int));

ERROR enableFullBuffering(FILE *fp);

int waitForByte();
int readEscapeKey();
int readKey();

#ifdef CURSOR_IMPL

    #include <termios.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <errno.h>
    #include <sys/ioctl.h>
    #include <stdlib.h>
    #include <signal.h>
    #include "error.h"

    static struct termios originalTermios;

    ERROR enableRawMode()
    {
        if (tcgetattr(STDIN_FILENO, &originalTermios) == -1)
            return "tcgetattr error";

        struct termios raw = originalTermios;
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;
        raw.c_cflag |= (CS8);
        raw.c_oflag &= ~(OPOST);
        raw.c_iflag &= ~(IXON | ICRNL | ISTRIP | INPCK | BRKINT);
        raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
            return "tcsetattr error";

        return OK;
    }

    ERROR disableRawMode()
    {
        if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios) == -1)
            return "tcsetattr error";
        return OK;
    }

    void disableLineWrap()
    {
        printf("\e[?7l");
    }

    void enableLineWrap()
    {
        printf("\e[?7h");
    }

    void hideCursor()
    {
        printf("\e[?25l");
    }

    void showCursor()
    {
        printf("\e[?25h");
    }

    void setCursorStyle(enum CURSOR_STYLE style)
    {
        printf("\e[%d q", (int)style);
    }

    ERROR getCursorPos(int *x, int *y)
    {
        printf("\033[6n");
        if (scanf("\e[%d;%dR", y, x) == 2)
            return OK;
        return SCANF_ERROR;
    }

    void setCursorPos(int x, int y)
    {
        printf("\e[%d;%dH", y ,x);
    }

    void setCursorX(int x)
    {
        printf("\e[%dG", x);
    }

    void cursorUp(int n)
    {
        printf("\e[%dA", n);
    }

    void cursorDown(int n)
    {
        printf("\e[%dB", n);
    }

    void cursorRight(int n)
    {
        printf("\e[%dC", n);
    }

    void cursorLeft(int n)
    {
        printf("\e[%dD", n);
    }

    void saveCursorPos()
    {
        printf("\e[s");
    }

    void restoreCursorPos()
    {
        printf("\e[u");
    }

    void enterAlternativeScreen()
    {
        printf("\e[?1049h");
    }

    void exitAlternativeScreen()
    {
        printf("\e[?1049l");
    }

    void clearLine()
    {
        printf("\e[K");
    }

    void clearScreen()
    {
        printf("\e[2J");
    }

    ERROR getScreenSizeByCursor(int *width, int *height)
    {
        setCursorPos(999, 999);
        return getCursorPos(width, height);
    }

    ERROR getScreenSizeByIoctl(int *width, int *height)
    {
        struct winsize ws;

        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0)
            return "ioctl error";

        *width = ws.ws_col;
        *height = ws.ws_row;
        return OK;
    }

    ERROR getScreenSize(int *width, int *height)
    {
        if (getScreenSizeByIoctl(width, height) == OK)
            return OK;

        return getScreenSizeByCursor(width, height);
    }

    ERROR registerChangeInWindowSize(void (*funciton)(int))
    {
        struct sigaction sa;
        sa.sa_handler = funciton;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);

        if (sigaction(SIGWINCH, &sa, NULL) == -1)
            return "sigaction error";

        return OK;
    }

    ERROR enableFullBuffering(FILE *fp)
    {
        // do not flush on '\n'
        if (setvbuf(fp, NULL, _IOFBF, BUFSIZ) != 0)
            return "setvbuf error";
        return OK;
    }

    int waitForByte()
    {
        char c;
        while (read(STDIN_FILENO, &c, 1) != 1);
        return c;
    }

    #define SEQUENCE(a, b) ((unsigned short)((((unsigned short)b) << 8) | a))

    int readEscapeKey()
    {
        unsigned short key;

        if (read(STDIN_FILENO, &key, 2) != 2)
            return '\e';

        switch (key) {
            case SEQUENCE('[', 'A'): return ARROW_UP;
            case SEQUENCE('[', 'B'): return ARROW_DOWN;
            case SEQUENCE('[', 'C'): return ARROW_RIGHT;
            case SEQUENCE('[', 'D'): return ARROW_LEFT;
            case SEQUENCE('[', '1'): return HOME; // might be with a ~
            case SEQUENCE('[', '5'): return PAGE_UP; // might be with a ~
            case SEQUENCE('[', '6'): return PAGE_DOWN; // might be with a ~
        }

        return '\e';
    }

    int readKey()
    {
        char c = waitForByte();
        if (c == '\e')
            return readEscapeKey();
        return c;
    }

#endif

#endif
