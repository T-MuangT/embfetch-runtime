#include "embfetch_config.h"

#ifndef LOGO_H
#define LOGO_H

#define ANSI_RED    "\033[31m"
#define ANSI_GREEN  "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_BLUE  "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN   "\033[36m"
#define ANSI_RESET "\033[0m"
#define NULL ((const char *)0)

#if defined(EMBFETCH_CPYTHON)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    ANSI_BLUE "            CCCCCCCCCCCCCCC             " ANSI_RESET,
    ANSI_BLUE "          CCCCCCCCCCCCCCCCCCC           " ANSI_RESET,
    ANSI_BLUE "          CCC   CCCCCCCCCCCCCC          " ANSI_RESET,
    ANSI_BLUE "          CCCCCCCCCCCCCCCCCCCC          " ANSI_RESET,
    ANSI_BLUE "          CCCCCCCCCCCCCCCCCCCC          " ANSI_RESET,
    ANSI_BLUE "     CCCCCCCCCCCCCCCCCCCCCCCCC" ANSI_YELLOW ":::::::" ANSI_RESET "    ",
    ANSI_BLUE "  CCCCCCCCCCCCCCCCCCCCCCCCCCCC" ANSI_YELLOW "::::::::::" ANSI_RESET "  ",
    ANSI_BLUE " CCCCCCCCCCCCCCCCCCCCCCCCCCCCC" ANSI_YELLOW ":::::::::::" ANSI_RESET,
    ANSI_BLUE "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC" ANSI_YELLOW "::::::::::::" ANSI_RESET,
    ANSI_BLUE "CCCCCCCCCCCCCCCCCCCCCCCCCCCCC " ANSI_YELLOW "::::::::::::" ANSI_RESET,
    ANSI_BLUE "CCCCCCCCCCCCCCCCCCCCCCCCCCC " ANSI_YELLOW "::::::::::::::" ANSI_RESET,
    ANSI_BLUE "CCCCCCCCCCCC" ANSI_YELLOW "::::::::::::::::::::::::::::::" ANSI_RESET,
    ANSI_BLUE "CCCCCCCCCC" ANSI_YELLOW "::::::::::::::::::::::::::::::::" ANSI_RESET,
    ANSI_BLUE "CCCCCCCCCC" ANSI_YELLOW "::::::::::::::::::::::::::::::::" ANSI_RESET,
    ANSI_BLUE " CCCCCCCCC" ANSI_YELLOW "::::::::::::::::::::::::::::::: " ANSI_RESET,
    ANSI_BLUE "  CCCCCCCC" ANSI_YELLOW "::::::::::::::::::::::::::::::  " ANSI_RESET,
    ANSI_YELLOW "          ::::::::::                        " ANSI_RESET,
    ANSI_YELLOW "          ::::::::::::::::::::              " ANSI_RESET,
    ANSI_YELLOW "          ::::::::::::::::::::              " ANSI_RESET,
    ANSI_YELLOW "          ::::::::::::::   :::              " ANSI_RESET,
    ANSI_YELLOW "           ::::::::::::::::::               " ANSI_RESET,
    ANSI_YELLOW "             ::::::::::::::                 " ANSI_RESET,
    NULL
};

#elif defined(EMBFETCH_MICROPYTHON)

#define LOGO_INFO_COLUMN "\033[40G"
const char *logo[] = {
    "+++++++++++++++ +++++++++++++++",
    "+++++++++++++++ +++++++++++++++",
    "+++++++++++++++ +++++++++++++++",
    "+++++++++++++++ +++++++++++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++ +++++++ +++++++",
    "+++++++ +++++++++++++++ +++++++",
    "+++++++ +++++++++++++++ +++ +++",
    "+++++++ +++++++++++++++ +++++++",
    "+++++++ +++++++++++++++ +++++++",
    NULL
};

#elif defined(EMBFETCH_CIRCUITPYTHON)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    "              @@@@@@@@                  ",
    "         @@@@@@@@ @@@@@                 ",
    "         @@@@@@@@@@@@@@                 ",
    "@@@@@@@@@@@@@@@@@@@@@@@                 ",
    "           @@@@@@@@@@@@                 ",
    "               @@@@@@@@                 ",
    "                 @@@@@@                 ",
    "                @@@@@@@                 ",
    "                @@@@@@                  ",
    "               @@@@@@@  @@@@@@@         ",
    "               @@@@@@ @@@@@@@@@@        ",
    "              @@@@@@@@@@@@@@@@@@@       ",
    "              @@@@@@@@@@@@@@@@@@        ",
    "              @@@@@@@@@@@@@@@@@@        ",
    "             @@@@@@@@@@@@@@@@@@       @@",
    "             @@@@@@@@@@@@@@@@@@     %@@@",
    "             @@@@@@@@@@ @@@@@@    @@@@@ ",
    "             @@@@@@@@@  @@@@@@@@@@@@@   ",
    "              @@@@@@     @@@@@@@@@@     ",
    NULL
};

#elif defined(EMBFETCH_OPENJDK)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    ANSI_MAGENTA "                    -===-.                   " ANSI_RESET ,
    ANSI_MAGENTA "                 .+*******+.                 " ANSI_RESET ,
    ANSI_MAGENTA "                .+*********+.                " ANSI_RESET ,
    ANSI_MAGENTA "                +***********+.               " ANSI_RESET ,
    ANSI_MAGENTA "               =*************+               " ANSI_RESET ,
    ANSI_MAGENTA "              =*************##=              " ANSI_RESET ,
    ANSI_MAGENTA "             -*************####=             " ANSI_RESET ,
    ANSI_MAGENTA "            :*************######-            " ANSI_RESET ,
    ANSI_MAGENTA "           .*************########:           " ANSI_RESET ,
    ANSI_MAGENTA "           *************##########.          " ANSI_RESET ,
    ANSI_MAGENTA "          +************############          " ANSI_RESET ,
    ANSI_MAGENTA "         +************%############*         " ANSI_RESET ,
    ANSI_MAGENTA "        =************#%%############*        " ANSI_RESET ,
    ANSI_MAGENTA "       -************#%%%%############+       " ANSI_RESET ,
    ANSI_MAGENTA "      -************#%%%%%#############=      " ANSI_RESET ,
    ANSI_MAGENTA "     :+************%%%%%%%#############-     " ANSI_RESET ,
    ANSI_MAGENTA "    .+************#%%%%%%%%#############:    " ANSI_RESET ,
    ANSI_MAGENTA "    :***********=. :+#%%%%%%############-    " ANSI_RESET ,
    ANSI_MAGENTA "    .+*********+.      .+*%%%#######%%##:    " ANSI_RESET ,
    ANSI_MAGENTA "     .+******+=.           .=*##%%%%%%#:     " ANSI_RESET ,
    ANSI_MAGENTA "       .-===-                   -+++=.       " ANSI_RESET ,
    NULL
};

#elif defined(EMBFETCH_ORACLE_JDK)

#define LOGO_INFO_COLUMN "\033[50G"
const char *logo[] = {
    ANSI_YELLOW "                         =              " ANSI_RESET ,
    ANSI_YELLOW "                         ==             " ANSI_RESET ,
    ANSI_YELLOW "                         ==             " ANSI_RESET ,
    ANSI_YELLOW "                        ==              " ANSI_RESET ,
    ANSI_YELLOW "                      ====              " ANSI_RESET ,
    ANSI_YELLOW "                    ====      =         " ANSI_RESET ,
    ANSI_YELLOW "                  =====  =====          " ANSI_RESET ,
    ANSI_YELLOW "               -=====  =====            " ANSI_RESET ,
    ANSI_YELLOW "              =====  ====               " ANSI_RESET ,
    ANSI_YELLOW "              ====  ====                " ANSI_RESET ,
    ANSI_YELLOW "             ====   ====                " ANSI_RESET ,
    ANSI_YELLOW "              ===   =====               " ANSI_RESET ,
    ANSI_YELLOW "               ===   =====              " ANSI_RESET ,
    ANSI_YELLOW "                ===   ====              " ANSI_RESET ,
    ANSI_YELLOW "                  ==   ===              " ANSI_RESET ,
    ANSI_BLUE "        ++++++        " ANSI_YELLOW "==-       " ANSI_BLUE "++++++  " ANSI_RESET ,
    ANSI_BLUE "      ++++++++++++++++++++++++      +++ " ANSI_RESET ,
    ANSI_BLUE "       ++++++++++++++++++          ++++ " ANSI_RESET ,
    ANSI_BLUE "          ++                      +++   " ANSI_RESET ,
    ANSI_BLUE "          ++++++++++++++++++   ++++     " ANSI_RESET ,
    ANSI_BLUE "            ++++++++++++      +         " ANSI_RESET ,
    ANSI_BLUE "           +++          ++              " ANSI_RESET ,
    ANSI_BLUE "           +++++++++++++++++            " ANSI_RESET ,
    ANSI_BLUE "    +++++     +++++++++++          +    " ANSI_RESET ,
    ANSI_BLUE "  ++++++++                    ++++++    " ANSI_RESET ,
    ANSI_BLUE "  +++++++++++++++++++++++++++++++ ++++  " ANSI_RESET ,
    ANSI_BLUE "           +++++++  ++++++++++++++++    " ANSI_RESET ,
    ANSI_BLUE "            +++++++++++++++++           " ANSI_RESET ,
    NULL

#endif // 

#endif /* LOGO_H */