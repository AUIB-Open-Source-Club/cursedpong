#ifndef GAME_H
#define GAME_H
#include <ncurses.h>

extern int match_ongoing;
extern int winner;

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

void init_paddle_params(PaddleParams* pr);

void init_ball_params(BallParams* pr);

void spawnPaddle(PaddleParams* p, bool flag, bool right);

void spawnBall(BallParams* b, PaddleParams* lp, PaddleParams* rp, ScoreParams* s);

void checkScore(ScoreParams* s);

void winCondition(ScoreParams* s);

void gameLoop();

#endif // !GAME_H
