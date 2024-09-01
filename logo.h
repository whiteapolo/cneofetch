#ifndef LOGO_H
#define LOGO_H

#include "string.h"
#define C0  "\e[0m"     /*  RESET        */

#define C1  "\e[0;31m"  /*  RED          */
#define C2  "\e[0;32m"  /*  GREEN        */
#define C3  "\e[0;33m"  /*  YELLOW       */
#define C4  "\e[0;34m"  /*  BLUE         */
#define C5  "\e[0;035m" /*  MAGENTA      */
#define C6  "\e[0;36m"  /*  CYAN         */
#define C7  "\e[0;37m"  /*  WHITE        */
#define C8  "\e[0;90m"  /*  GRAY         */

#define B1  "\e[1;91m"  /*  BOLD RED     */
#define B2  "\e[1;92m"  /*  BOLD GREEN   */
#define B3  "\e[1;93m"  /*  BOLD YELLOW  */
#define B4  "\e[1;94m"  /*  BOLD BLUE    */
#define B5  "\e[1;95m"  /*  BOLD MAGENTA */
#define B6  "\e[1;96m"  /*  BOLD CYAN    */
#define B7  "\e[1;97m"  /*  BOLD WHITE   */
#define B8  "\e[1;90m"  /*  BOLD GRAY    */

const char penguin[] = {
    "        "C8"#####"C0"\n"
    "       "C8"#######"C0"\n"
    "       "C8"##"C0"0"C8"#"C0"0"C8"##"C0"\n"
    "       "C8"#"C3"#####"C8"#"C0"\n"
    "     "C8"##"C0"##"C3"###"C0"##"C8"##"C0"\n"
    "    "C8"#"C0"##########"C8"##"C0"\n"
    "   "C8"#"C0"############"C8"##"C0"\n"
    "   "C8"#"C0"###########"C8"####"C0"\n"
    "  "C3"##"C8"#"C0"###########"C8"##"C3"#"C0"\n"
    ""C3"######"C8"#"C0"#######"C8"#"C3"######"C0"\n"
    ""C3"#######"C8"#"C0"#####"C8"#"C3"#######"C0"\n"
    "  "C3"#####"C8"#######"C3"#####"C0"\n"
};

const char star[] = {
    B1"       ,O,         \n"
    B2"      ,OOO,        \n"
    B3"'oooooOOOOOooooo'  \n"
    B4"  `OOOOOOOOOOO`    \n"
    B5"    `OOOOOOO`      \n"
    B6"    OOOO'OOOO      \n"
    B7"   OOO'   'OOO     \n"
    B8"  O'         'O    \n"
};

const char emptyLogo[] = {
    "\n"
};

const char *LOGO = penguin;

#endif
