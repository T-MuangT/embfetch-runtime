#ifndef LOGO_H
#define LOGO_H

#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_RESET   "\033[0m"

#define LOGO_INFO_COLUMN "\033[50G"

static const char *logo[] = {
    ANSI_YELLOW "            CCCCCCCCCCCCCCC             " ANSI_RESET,
    ANSI_YELLOW "          CCCCCCCCCCCCCCCCCCC           " ANSI_RESET,
    ANSI_YELLOW "          CCC   CCCCCCCCCCCCCC          " ANSI_RESET,
    ANSI_YELLOW "          CCCCCCCCCCCCCCCCCCCC          " ANSI_RESET,
    ANSI_YELLOW "          CCCCCCCCCCCCCCCCCCCC          " ANSI_RESET,
    ANSI_YELLOW "     CCCCCCCCCCCCCCCCCCCCCCCCC" ANSI_BLUE "::::::" ANSI_RESET "    ",
    ANSI_YELLOW "  CCCCCCCCCCCCCCCCCCCCCCCCCCCC" ANSI_BLUE "::::::::" ANSI_RESET "  ",
    ANSI_YELLOW " CCCCCCCCCCCCCCCCCCCCCCCCCCCCC" ANSI_BLUE "::::::::::" ANSI_RESET,
    ANSI_YELLOW "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC" ANSI_BLUE "::::::::::" ANSI_RESET,
    ANSI_YELLOW "CCCCCCCCCCCCCCCCCCCCCCCCCCCCC " ANSI_BLUE "::::::::::" ANSI_RESET,
    ANSI_YELLOW "CCCCCCCCCCCCCCCCCCCCCCCCCCC " ANSI_BLUE "::::::::::::" ANSI_RESET,
    ANSI_YELLOW "CCCCCCCCCCCC" ANSI_BLUE "::::::::::::::::::::::::::::::::" ANSI_RESET,
    ANSI_YELLOW "CCCCCCCCCC" ANSI_BLUE "::::::::::::::::::::::::::::::::::" ANSI_RESET,
    ANSI_YELLOW "CCCCCCCCCC" ANSI_BLUE "::::::::::::::::::::::::::::::::::" ANSI_RESET,
    ANSI_YELLOW " CCCCCCCCC" ANSI_BLUE "::::::::::::::::::::::::::::::::: " ANSI_RESET,
    ANSI_YELLOW "  CCCCCCCC" ANSI_BLUE "::::::::::::::::::::::::::::::::  " ANSI_RESET,
    ANSI_BLUE "          ::::::::::                        " ANSI_RESET,
    ANSI_BLUE "          ::::::::::::::::::::              " ANSI_RESET,
    ANSI_BLUE "          ::::::::::::::::::::              " ANSI_RESET,
    ANSI_BLUE "          ::::::::::::::   :::              " ANSI_RESET,
    ANSI_BLUE "           ::::::::::::::::::               " ANSI_RESET,
    ANSI_BLUE "             ::::::::::::::                 " ANSI_RESET,
    NULL
};

#endif // LOGO_H