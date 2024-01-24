#ifndef CONFIG_H
#define CONFIG_H

#define CPU_MODELWORDS 3

static const char package_query[][2][256] = {
{"(dpkg)", "grep -sc \"^Package: \" /var/lib/dpkg/status"},
{"(pacman)", "ls -f /var/lib/pacman/local 2>/dev/null | wc -l"},
{"(flatpak)", "find /var/lib/flatpak/app /var/lib/flatpak/runtime -mindepth 1 -maxdepth 1 -type d 2>/dev/null | wc -l"},
{"(xbps)", "xbps-query -l 2>/dev/null | wc -l"}
};

#define C0         "\e[0m"     /*  RESET   */

#define C1         "\e[0;31m"    /*  RED     */
#define C2         "\e[0;32m"    /*  GREEN   */
#define C3         "\e[0;33m"    /*  YELLOW  */
#define C4         "\e[0;34m"    /*  BLUE    */
#define C5         "\e[0;035m"   /*  MAGENTA */
#define C6         "\e[0;36m"    /*  CYAN    */
#define C7         "\e[0;37m"    /*  WHITE   */
#define C8         "\e[0;90m"    /*  GRAY    */

#define B1        "\e[1;91m"   /*  BOLD RED     */
#define B2        "\e[1;92m"   /*  BOLD GREEN   */
#define B3        "\e[1;93m"   /*  BOLD YELLOW  */
#define B4        "\e[1;94m"   /*  BOLD BLUE    */
#define B5        "\e[1;95m"   /*  BOLD MAGENTA */
#define B6        "\e[1;96m"   /*  BOLD CYAN    */
#define B7        "\e[1;97m"   /*  BOLD WHITE   */
#define B8        "\e[1;90m"   /*  BOLD GRAY    */

#define LOGO penguin

#define penguin                                         \
"         "C8"#####"C0"                              \n"\
"        "C8"#######"C0"                             \n"\
"        "C8"##"C0"0"C8"#"C0"0"C8"##"C0"             \n"\
"        "C8"#"C3"#####"C8"#"C0"                     \n"\
"      "C8"##"C0"##"C3"###"C0"##"C8"##"C0"           \n"\
"     "C8"#"C0"##########"C8"##"C0"                  \n"\
"    "C8"#"C0"############"C8"##"C0"                 \n"\
"    "C8"#"C0"###########"C8"####"C0"                \n"\
"   "C3"##"C8"#"C0"###########"C8"##"C3"#"C0"        \n"\
" "C3"######"C8"#"C0"#######"C8"#"C3"######"C0"      \n"\
" "C3"#######"C8"#"C0"#####"C8"#"C3"#######"C0"      \n"\
"   "C3"#####"C8"#######"C3"#####"C0"                \n"\


#define star             \
B8"	.             \n"\
B1"       ,O,         \n"\
B2"      ,OOO,        \n"\
B3"'oooooOOOOOooooo'  \n"\
B4"  `OOOOOOOOOOO`    \n"\
B5"    `OOOOOOO`      \n"\
B6"    OOOO'OOOO      \n"\
B7"   OOO'   'OOO     \n"\
B8"  O'         'O    \n"\


#define no_logo \
"\n"            \
"\n"            \
"\n"            \
"\n"            \
"\n"            \
"\n"            \
"\n"            \
"\n"            \
/* █ */
/* ▒ */
 

#define pic \
"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒█████████▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒█████████▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒█████████▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒█████████▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒█████████▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n"\
"▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\n"\


#endif
