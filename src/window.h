#ifndef WINDOW_H
#define WINDOW_H
#include <ncurses.h>

#define COLOR_BG_BLUE       1
#define COLOR_DIALOG_GRAY   2
#define COLOR_SHADOW_BLACK  3
#define COLOR_TEXT_BLUE     4
#define COLOR_BG_RED        5
#define COLOR_BORDER_DARK   6
#define COLOR_BORDER_LIGHT  7

WINDOW* newWindow(int height, int width, int starty, int startx, char* label);
void printASCII();

#endif // !WINDOW_H
