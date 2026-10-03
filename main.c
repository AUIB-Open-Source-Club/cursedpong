#include <curses.h>
#include <ncurses.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include "controls.h"

#define COLOR_BG_BLUE       1
#define COLOR_DIALOG_GRAY   2
#define COLOR_SHADOW_BLACK  3
#define COLOR_TEXT_BLUE     4
#define COLOR_BG_RED        5
#define COLOR_BORDER_DARK   6
#define COLOR_BORDER_LIGHT  7

typedef struct {
    WINDOW* mainwin;
    int current_y, current_x;
    int height, width;
    int ydirection;
} PaddleParams;

typedef struct {
    WINDOW* mainwin;
    int current_y, current_x;
    int height, width;
    double yfactor;
    int bounceRate;
    int xdirection;
    int ydirection;
} BallParams;

typedef struct {
    int p1score;
    int p2score;
} ScoreParams ;

static void init_paddle_params(PaddleParams* pr){
    pr->height = getmaxy(pr->mainwin);
    pr->width = getmaxx(pr->mainwin);
    pr->current_y = pr->height/2;
    pr->current_x = pr->width/2;
    pr->ydirection = 0;
}

static void init_ball_params(BallParams* pr){
    pr->height = getmaxy(pr->mainwin);
    pr->width = getmaxx(pr->mainwin);
    pr->current_y = pr->height/2;
    pr->current_x = pr->width/2;
    pr->yfactor = 0;
    pr->bounceRate = 4;
    pr->xdirection = -1;
    pr->ydirection = 1;
}

static void checkScore(ScoreParams* s);

// Now it's time to get funky.
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

static void spawnPaddle(PaddleParams* p, bool flag, bool right){
    int length = 4;
    int pos;

    if (right) {
        pos = p->width-3;
        p->current_x = pos - 1;
    } else {
        pos = 1;
        p->current_x = 3;
    }

    //if (win->current_y == 0) {
    //    win->current_y++;
    //    return;
    //}
    //if (win->current_y == win->height - length) {
    //    win->current_y--;
    //    return;
    //}

    if (flag == TRUE) {
        for (int i = 0; i < length; ++i) {
            mvwprintw(p->mainwin, p->current_y+i, pos, "\xe2\x96\x88\xe2\x96\x88");
            //if(win->current_y != win->height)
            mvwprintw(p->mainwin, p->current_y+i+1, pos, "  ");
            p->ydirection = -1;
        }
    } else {
        for (int i = 0; i < length; ++i) {
            mvwprintw(p->mainwin, p->current_y+i, pos, "\xe2\x96\x88\xe2\x96\x88");
            //if(win->current_y != 1)
            mvwprintw(p->mainwin, p->current_y-1, pos, "  ");
            p->ydirection = 1;
        }
    }

    wrefresh(p->mainwin);
}

static void spawnBall(BallParams* b, PaddleParams* lp, PaddleParams* rp, ScoreParams* s){

    mvwprintw(b->mainwin, b->current_y, b->current_x, " ");

    b->yfactor += (double)b->ydirection / b->bounceRate;
    int factor = (int)trunc(b->yfactor);
    if (factor != 0) {
        b->current_y += factor;
        b->yfactor -= (double)factor;
    }

    b->current_x += b->xdirection;

    if (b->current_y >= b->height-1) {
        b->current_y = b->height - 2;
        b->yfactor = 0;
        b->ydirection = -1;
        b->current_y += -1;
    } else if (b->current_y <= 0) {
        b->current_y = 1;
        b->yfactor = 0;
        b->ydirection = 1;
        b->current_y += 1;
    }

    if (b->xdirection == -1 &&
            (b->current_y >= lp->current_y && b->current_y < lp->current_y + 4)
            && b->current_x == lp->current_x){
        b->xdirection *= -1;
        if (lp->ydirection == b->ydirection) {
            if (b->bounceRate != 1) {
                --b->bounceRate;
            }
        }
        else {
            ++b->bounceRate;
        }
    }

    else if (b->xdirection == 1 &&
            (b->current_y >= rp->current_y && b->current_y < rp->current_y + 4)
            && b->current_x == rp->current_x){
        b->xdirection *= -1;
        if (rp->ydirection == b->ydirection) {
            if (b->bounceRate != 1) {
                --b->bounceRate;
            }
        }
        else ++b->bounceRate;
    }

    if (b->current_x == b->width-2 && b->xdirection == 1) {
        mvwprintw(stdscr, LINES-2, 0, "Player 1 Scored");
        b->current_x = b->width/2;
        b->current_y = b->height/2;
        b->xdirection *= -1;
        b->bounceRate = 4;
        s->p1score++;
        checkScore(s);
    } else if (b->current_x == 1 && b->xdirection == -1) {
        mvwprintw(stdscr, LINES-2, 0, "Player 2 Scored");
        b->current_x = b->width/2;
        b->current_y = b->height/2;
        b->xdirection *= -1;
        b->bounceRate = 4;
        s->p2score++;
        checkScore(s);
    }

    mvwprintw(b->mainwin, b->current_y, b->current_x, "\xe2\x97\x8f");

    wrefresh(b->mainwin);
}

static void checkScore(ScoreParams* s){
    int mini_height = 10;
    int mini_width = 20;
    WINDOW* p1 = newWindow(mini_height,
                                mini_width,
                                (LINES/2) + 10,
                                ((COLS / 2) - mini_width) - 1, 
                                "Player 1");
    wrefresh(p1);
    refresh();
    WINDOW* p2 = newWindow(mini_height,
                                mini_width,
                                (LINES/2) + 10,
                                (COLS / 2) + 1, 
                                "Player 2");
    wrefresh(p2);
    refresh();

    int y1, y2, x1, x2;
    getmaxyx(p1, y1, x1);
    getmaxyx(p2, y2, x2);

    attron(COLOR_PAIR(COLOR_TEXT_BLUE));
    mvwprintw(p1, y1/2, 1, "Score:   %d", s->p1score);
    mvwprintw(p2, y2/2, 1, "Score:   %d", s->p2score);
    attroff(COLOR_PAIR(COLOR_TEXT_BLUE));
    wrefresh(p1);
    wrefresh(p2);
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

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
    mvaddstr(1, 1, "Pong (Beta)");
    refresh();

    int main_height = 20;
    int main_width = 70;
    int main_center_y = (row - main_height) / 2;
    int main_center_x = (col - main_width) / 2;

    int mini_height = 10;
    int mini_width = 20;
    int mini_centery = (row - mini_height) / 2;
    int mini_centerx = (col - mini_width) / 2;

    PaddleParams paddle1;
    PaddleParams paddle2;
    WINDOW* mainwin = newWindow(main_height, main_width, main_center_y, main_center_x, "Pong!");
    refresh();


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
    while (1) {
        //flushinp(); // Cuz the paddles keep LAGGING, DAMN!!!!!!!!!!!!!!!!!
                    // Essentially, this flushes the input buffer for ncurses,
                    //              getting rid of the "inertia" effect
                    //              (Flushing every frame sounds stupid,
                    //              will think of a better solution later... TODO)
                    //              ((Done))

        // Collision check (paddles)
        if (paddle1.current_y == 1) {
            if (key_w_pressed) continue; 
        } else if (paddle1.current_y == paddle1.height - 5) {
            if (key_s_pressed) continue;
        }
        if (paddle2.current_y == 1) {
            if (key_up_pressed) continue;
        } else if (paddle2.current_y == paddle2.height - 5) {
            if (key_dn_pressed) continue;
        }
        if (key_up_pressed) { 
                --paddle1.current_y;
                spawnPaddle(&paddle1, TRUE, FALSE);
                flushinp(); // 160 IQ move
                break;
        }
    //            ++paddle1.current_y;
    //            spawnPaddle(&paddle1, FALSE, FALSE);
    //            flushinp();
    //            break;
    //        case KEY_UP:
    //            --paddle2.current_y;
    //            spawnPaddle(&paddle2, TRUE, TRUE);
    //            flushinp();
    //            break;
    //        case KEY_DOWN:
    //            ++paddle2.current_y;
    //            spawnPaddle(&paddle2, false, TRUE);
    //            flushinp();
    //            break;
        
        mvwprintw(stdscr, 0, 0, "Rate: %d", ball.bounceRate);
        spawnBall(&ball, &paddle1, &paddle2, &score);
        usleep(30000);
        wrefresh(mainwin);
    }
    endwin();
    return EXIT_SUCCESS;
}
