#include <string.h>
#include "window.h"

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

    mvwprintw(win, 0, (x - strlen(label))/2, " %s", label);

    wrefresh(win);
    return win;
}

void printASCII(){
    WINDOW* asciiWin = newwin(5, COLS, 4, (COLS-44)/2);
    wbkgd(asciiWin, COLOR_PAIR(COLOR_BG_BLUE));
    wrefresh(asciiWin);
    FILE* ascii;
    ssize_t read;
    char* line = NULL;
    size_t len;

    ascii = fopen("./ascii.txt", "r");
    if (!ascii) {
        printf("ASCII file missing or failed to open.\n");
        return;
    }
    while ((read = getline(&line, &len, ascii)) != -1){
        move(10, COLS/2);
        wprintw(asciiWin,"%s", line);
        refresh();
        wrefresh(asciiWin);
    }
    refresh();
    wrefresh(stdscr);
    fclose(ascii);
}

