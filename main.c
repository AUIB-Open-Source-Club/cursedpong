#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <locale.h>

#define COLOR_BG_BLUE       1
#define COLOR_DIALOG_GRAY   2
#define COLOR_SHADOW_BLACK  3
#define COLOR_TEXT_BLUE     4
#define COLOR_BG_RED        5

typedef struct {
    WINDOW* win;
    int startx, starty;
    int height, width;
} Props;

WINDOW* newWindow(int height, int width, int starty, int startx){

    WINDOW* mainWin = newwin(height, width, starty, startx);
    wbkgd(mainWin, COLOR_PAIR(COLOR_DIALOG_GRAY));

    int y, x;
    getmaxyx(mainWin, y, x);

    box(mainWin, 0, 0);

    attron(COLOR_PAIR(3));
    for (int i = 1; i < height+1; ++i) {
        mvprintw(i+starty, width+startx, " "); 
    }
    for (int j = 1; j < width+1; ++j) {
        mvprintw(height+starty, j+startx, " "); 
    }
    attroff(COLOR_PAIR(3));

    mvwprintw(mainWin, 0, (x/2), "Nice");

    wrefresh(mainWin);
    return mainWin;
}

static void spawnPaddle(WINDOW* win, Props* prop, bool flag){

    int y, x;
    y = getmaxy(win);

    if (prop->height == 0) {
        prop->height -= 1; 
        return;
    }
    prop->starty = y / 2;

    int length = 4;

    if (flag == TRUE) {
        for (int i = 0; i < length; ++i) {
            mvwprintw(win, prop->height+i, 1, "[]");
        }
    } else {
        for (int i = 0; i < length; ++i) {
            mvwprintw(win, prop->height+i, 1, "  ");
        }
    }

    wrefresh(win);
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

    Props paddleprops;
    int row, col;

    initscr();
    cbreak();
    noecho();
    //nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0); 
    getmaxyx(stdscr, row, col);

    if (!has_colors()) {
        endwin();
        printf("Terminal doesn't support colors\n");
        return EXIT_FAILURE;
    }
    start_color();

    init_pair(COLOR_BG_BLUE,       COLOR_WHITE, COLOR_BLUE);
    init_pair(COLOR_DIALOG_GRAY,   COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_SHADOW_BLACK,  COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_TEXT_BLUE,     COLOR_BLUE,  COLOR_WHITE);
    init_pair(COLOR_BG_RED,     COLOR_WHITE,  COLOR_RED);

    bkgd(COLOR_PAIR(COLOR_BG_RED));
    mvaddstr(1, 1, "Pong (Beta)");
    refresh();

    int height = 20;
    int width = 70;
    int center_y = (row-height) / 2;
    int center_x = (col-width) / 2;

    WINDOW* gamewin = newWindow(height, width, center_y, center_x);

    int ch;

    paddleprops.height = -1;
    spawnPaddle(gamewin, &paddleprops, TRUE);
    while ((ch = getch()) != KEY_F(1)) {

        switch (ch) {
            case KEY_UP:
                spawnPaddle(gamewin, &paddleprops, FALSE);
                --paddleprops.height;
                spawnPaddle(gamewin, &paddleprops, TRUE);
                break;
            case KEY_DOWN:
                spawnPaddle(gamewin, &paddleprops, FALSE);
                ++paddleprops.height;
                spawnPaddle(gamewin, &paddleprops, TRUE);
                break;
        }

    }

    endwin();
    return EXIT_SUCCESS;
}
