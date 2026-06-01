#ifndef CPYTHON_LOGO_H
#define CPYTHON_LOGO_H

#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_RESET   "\033[0m"

#define LOGO_INFO_COLUMN "\033[50G"

static const char *logo[] = {
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

#endif // CPYTHON_LOGO_H