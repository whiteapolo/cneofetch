#ifndef CURSORS_UTILS_H
#define CURSORS_UTILS_H

#define SAVE_CURSOR()           fputs("\033[s", stdout)
#define RESTORE_CURSOR()        fputs("\033[u", stdout)
#define CURSOR_DOWN(rows)       printf("\033[%dB", rows)
#define CURSOR_UP(rows)         printf("\033[%dA", rows)
#define CURSOR_RIGHT(columns)   printf("\033[%dC", columns)
#define CURSOR_LEFT(columns)    printf("\033[%dD", columns)

#define CURSOR_HIDE()          fputs("\033[?25l", stdout)
#define CURSOR_SHOW()          fputs("\033[?25h", stdout)

#endif
