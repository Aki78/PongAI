#ifndef PLAYER_H
#define PLAYER_H

#include "paddle.h"
class Game;

class Player
{
public:
    Player(Game& game, bool manual);
    unsigned int mScore;
    Paddle mPaddle;
    Game& mGame;
    bool mManual;
    QGraphicsTextItem mScoreText;

    void score();
    void reset();
    void positionScoreText();
};

#endif // PLAYER_H
