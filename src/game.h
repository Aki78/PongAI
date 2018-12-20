#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "paddle.h"
#include "wall.h"
#include "player.h"
#include "gameview.h"
#include "ballcontactlistener.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QObject>
#include <memory>
#include <Box2D/Dynamics/b2World.h>

class Game : public QObject
{
    Q_OBJECT

public:
    Game();
    void loop();
    b2World mWorld;
    QGraphicsScene mScene;
    Ball mBall;
    BallContactListener mListener;
    void* mPrevContact;
    std::vector<float> requestFrame(const std::vector<float>& input);
    std::vector<float> requestPosVel(const std::vector<float>& input);
    GameView mView;


private:
    Wall mWallBottom;
    Wall mWallTop;
    QTimer timer;
    QElapsedTimer elapsedTimer;
    Player mPlayer1;
    Player mPlayer2;
    float32 mTimeStep;
    int32 mVelocityIterations;
    int32 mPositionIterations;
    QGraphicsLineItem mLine;


public slots:
    void progress();
};

#endif // GAME_H
