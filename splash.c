#include "splash.h"
#include <stdlib.h>
#include <curses.h>

void create_splash_screen(WINDOW *stdscr) {
   initscr();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    const char* toth[] = {
        "TTTTT  OOO  TTTTT  H   H",
        "  T   O   O   T    H   H",
        "  T   O   O   T    HHHHH",
        "  T   O   O   T    H   H",
        "  T    OOO    T    H   H"
    };

    for (int i = 0; i < 30; ++i) {
        clear();
        if (i % 2 == 0) {
            attron(COLOR_PAIR(1));
            for (int j = 0; j < 5; ++j) {
                mvprintw(LINES / 2 - 5 + j, (COLS - 28) / 2, "%s", toth[j]);
            }
            attroff(COLOR_PAIR(1));
        }
        refresh();
        usleep(100000);
    }

    endwin();
}

