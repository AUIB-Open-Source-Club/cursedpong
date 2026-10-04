#ifndef WINDOW_H
#define WINDOW_H
#include <ncurses.h>

WINDOW* newWindow(int height, int width, int starty, int startx, char* label);

#endif // !WINDOW_H
