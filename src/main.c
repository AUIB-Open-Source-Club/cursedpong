#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <fcntl.h>

#include "window.h"
#include "game.h"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "en_US.UTF-8");

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0); 

    if (!has_colors()) {
        endwin();
        printf("Terminal doesn't support colors\n");
        return EXIT_FAILURE;
    }
    if (argc > 1) {
        if (strcmp(argv[1], "--nocolor") == 0){
            ;
        } else start_color();
        
    }
    else {
        start_color();
    }

    init_pair(COLOR_BG_BLUE,        COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_DIALOG_GRAY,    COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_SHADOW_BLACK,   COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_TEXT_BLUE,      COLOR_BLUE,  COLOR_WHITE);
    init_pair(COLOR_BG_RED,         COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_BORDER_DARK,    COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_BORDER_LIGHT,   COLOR_WHITE, COLOR_WHITE);

    bkgd(COLOR_PAIR(COLOR_BG_BLUE));
    mvaddstr(1, 1, "AUIB Open Source Club");
    refresh();

    gameLoop();

    int ch;
    while ((ch = getch()) != 10);
    
    endwin();
    return EXIT_SUCCESS;
}
