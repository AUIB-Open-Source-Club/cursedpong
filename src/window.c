#include <ncurses.h>
#include <string.h>

#define COLOR_BG_BLUE       1
#define COLOR_DIALOG_GRAY   2
#define COLOR_SHADOW_BLACK  3
#define COLOR_TEXT_BLUE     4
#define COLOR_BG_RED        5
#define COLOR_BORDER_DARK   6
#define COLOR_BORDER_LIGHT  7

WINDOW* newWindow(int height, int width, int starty, int startx, char* label){

    WINDOW* win = newwin(height, width, starty, startx);
    wbkgd(win, COLOR_PAIR(COLOR_DIALOG_GRAY));

    int y, x;
    getmaxyx(win, y, x);

    // Custom 3D box thingy idk
    wattron(win, COLOR_PAIR(COLOR_BORDER_LIGHT) | A_BOLD);
    mvwaddch(win, 0, 0, ACS_ULCORNER);
    mvwaddch(win, y - 1, 0, ACS_LLCORNER);
    mvwhline(win, 0, 1, ACS_HLINE, x-2);
    mvwvline(win, 1, 0, ACS_VLINE, y-2);
    wattroff(win, COLOR_PAIR(COLOR_BORDER_LIGHT | A_BOLD));

    wattrset(win, A_NORMAL);

    wattron(win, COLOR_PAIR(COLOR_BORDER_DARK) | A_BOLD);
    mvwaddch(win, 0, x-1, ACS_URCORNER);
    mvwaddch(win, y - 1, x-1, ACS_LRCORNER);
    mvwhline(win, y-1, 1, ACS_HLINE, x-2);
    mvwvline(win, 1, x-1, ACS_VLINE, y-2);
    wattroff(win, COLOR_PAIR(COLOR_BORDER_DARK) | A_BOLD);

    attron(COLOR_PAIR(3));
    for (int i = 1; i < height+1; ++i) {
        mvprintw(i+starty, width+startx, " "); 
    }
    for (int j = 1; j < width+1; ++j) {
        mvprintw(height+starty, j+startx, " "); 
    }
    attroff(COLOR_PAIR(3));

    mvwprintw(win, 0, (x - strlen(label))/2, " %s ", label);

    wrefresh(win);
    return win;
}

