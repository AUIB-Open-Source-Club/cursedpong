#include <curses.h>
#include <ncurses.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <fcntl.h>
#include <stdio.h>

#include "input.h"
#include "window.h"
#include "game.h"

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

    int kb = open("/dev/input/by-id/usb-Gaming_KB_Gaming_KB-event-kbd", O_RDONLY | O_NONBLOCK);
    if (kb < 0){
        perror("Error");
        return EXIT_FAILURE;
    }

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0); 

    int row, col;
    getmaxyx(stdscr, row, col);


    if (!has_colors()) {
        endwin();
        printf("Terminal doesn't support colors\n");
        return EXIT_FAILURE;
    }
    start_color();

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

    int main_height = 25;
    int main_width = 80;
    int main_center_y = (row - main_height) / 2;
    int main_center_x = (col - main_width) / 2;

    PaddleParams paddle1;
    PaddleParams paddle2;
    WINDOW* mainwin = newWindow(main_height, main_width, main_center_y, main_center_x, "Pong!");
    refresh();
    printASCII();


    paddle1.mainwin = mainwin;
    paddle2.mainwin = mainwin;
    init_paddle_params(&paddle1);
    init_paddle_params(&paddle2);

    BallParams ball;
    ball.mainwin = mainwin;
    init_ball_params(&ball);

    ScoreParams score = {0, 0};

    spawnPaddle(&paddle1, TRUE, FALSE);
    spawnPaddle(&paddle2, TRUE, TRUE);
    checkScore(&score);
    while (match_ongoing) {
        handle_input(kb);
        if (key_w_pressed) { 
            if (paddle1.current_y == 1) {
                flushinp();
            } else {
                --paddle1.current_y;
                spawnPaddle(&paddle1, TRUE, FALSE);
                flushinp(); // 160 IQ move
            }
        }
        if (key_s_pressed) { 
            if (paddle1.current_y == paddle1.height - 5) {
                flushinp();
            } else {
                ++paddle1.current_y;
                spawnPaddle(&paddle1, FALSE, FALSE);
                flushinp();
            }
        }
        if (key_up_pressed) { 
            if (paddle2.current_y == 1) {
                flushinp();
            } else {
                --paddle2.current_y;
                spawnPaddle(&paddle2, TRUE, TRUE);
                flushinp();
            }
        }
        if (key_dn_pressed) { 
            if (paddle2.current_y == paddle2.height - 5) {
                flushinp();
            } else {
                ++paddle2.current_y;
                spawnPaddle(&paddle2, FALSE, TRUE);
                flushinp();
            }
        }
        spawnBall(&ball, &paddle1, &paddle2, &score);
        usleep(30000);
        wrefresh(mainwin);
    }
    delwin(mainwin);
    clear();
    werase(stdscr);
    //wbkgd(stdscr, COLOR_PAIR(COLOR_BG_BLUE));
    wrefresh(stdscr);
    nodelay(stdscr, false);
    winCondition(&score);
    printASCII();

    int ch;
    while ((ch = getch()) != 10);
    
    endwin();
    return EXIT_SUCCESS;
}
