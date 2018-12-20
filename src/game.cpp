#include "game.h"
#include <QDebug>
#include <QOpenGLWidget>
#include <Box2D/Box2D.h>
#include "constants.h"


Game::Game()
    : QObject()
    , mWorld(b2Vec2(0.0f, 0.0f))
    , mScene(-pixelsPerMeter*windowWidth, -pixelsPerMeter*windowHeight, 2*pixelsPerMeter*windowWidth, 2*pixelsPerMeter*windowHeight)
    , mView(&mScene)
    , mBall(mWorld, mScene)
    , mListener(this)
    , mWallBottom(mWorld, mScene, QVector2D(-windowWidth, -windowHeight), QVector2D(windowWidth, -windowHeight))
    , mWallTop(mWorld, mScene, QVector2D(-windowWidth, windowHeight), QVector2D(windowWidth, windowHeight))
    , timer()
    , mPlayer1(*this, true)
    , mPlayer2(*this, false)
    , mTimeStep(1.0f / 600.0f)
    , mVelocityIterations(6)
    , mPositionIterations(2)
    , mLine(0, -pixelsPerMeter*windowHeight, 0, pixelsPerMeter*windowHeight)
{
    QOpenGLWidget *glWidget = new QOpenGLWidget();
    mView.setViewport(glWidget);
    mView.update();
    mView.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Pong"));
    mView.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    mView.setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    mView.setCacheMode(QGraphicsView::CacheBackground);
    mView.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    mView.show(); //visualization

    // Connect key signals from view to the paddle
    QObject::connect(&mView, &GameView::upPressed, &(mPlayer1.mPaddle), &Paddle::upPressed);
    QObject::connect(&mView, &GameView::downPressed, &(mPlayer1.mPaddle), &Paddle::downPressed);
    QObject::connect(&mView, &GameView::upReleased, &(mPlayer1.mPaddle), &Paddle::upReleased);
    QObject::connect(&mView, &GameView::downReleased, &(mPlayer1.mPaddle), &Paddle::downReleased);

    QObject::connect(&mView, &GameView::rightPressed, &(mPlayer1.mPaddle), &Paddle::rightPressed);
    QObject::connect(&mView, &GameView::leftPressed, &(mPlayer1.mPaddle), &Paddle::leftPressed);
    QObject::connect(&mView, &GameView::rightReleased, &(mPlayer1.mPaddle), &Paddle::rightReleased);
    QObject::connect(&mView, &GameView::leftReleased, &(mPlayer1.mPaddle), &Paddle::leftReleased);

    // Connect signal from timer to game progress
    QObject::connect(&this->timer, SIGNAL(timeout()), this, SLOT(progress()));

    // Stylize the line that divides the sides
    QColor scoreColor = QColor(255, 255, 255);
    QPen linePen = QPen(QBrush(scoreColor), pixelsPerMeter*0.5, Qt::DashLine);
    linePen.setDashPattern(QVector<qreal>({2, 4}));
    mLine.setPen(linePen);
    mScene.addItem(&mLine);

    // Set contact listener
    mWorld.SetContactListener(&mListener);

    // Set the starting configuration
    mBall.setPosition(b2Vec2(0, 0));
    float randY = 20*(float(rand())/float(RAND_MAX)-0.5);
    mBall.setVelocity(b2Vec2(-mBall.mInitialSpeed, randY));
    mBall.updateVisuals();

    mPlayer1.mPaddle.setPosition(b2Vec2(-40, 0));
    mPlayer2.mPaddle.updateVisuals();

    mPlayer2.mPaddle.setPosition(b2Vec2(40, 0));
    mPlayer2.mPaddle.updateVisuals();
}

void Game::loop()
{

    // Start the game loop
    timer.start(8); // 0 or 16, 0 -> fast
    elapsedTimer.start();
}

std::vector<float> Game::requestFrame(const std::vector<float>& input) {

    // Set the input
    float up = input[0];
    float down = input[1];
    if (up > 0.5) {
        this->mPlayer1.mPaddle.upPressed();
    } else {
        this->mPlayer1.mPaddle.upReleased();
    }
    if (down > 0.5) {
        this->mPlayer1.mPaddle.downPressed();
    } else {
        this->mPlayer1.mPaddle.downReleased();
    }

    // Advance the game by one frame
    progress();

    // Grab the pixels as output of the game and input for the learning
    QPixmap pixels = mView.grab();
    QImage img = pixels.toImage();
//    img.convertToFormat(QImage::Format_Mono);
    img.setPixelColor(0,0, QColor(255, 0, 0).rgb());
    img.setPixelColor(1,1, QColor(255, 0, 0).rgb());
    img.save("img.png");

    QRgb *st = (QRgb *) img.bits();
    quint64 pixelCount = img.width() * img.height();
    std::vector<float> inputs(pixelCount+1);
    for (quint64 p = 0; p < pixelCount; p++) {
        // st[p] has an individual pixel
        QColor a = st[p];
        a.toHsv();
        int value = a.value();
        float inputValue;
        if (value < 128) {
            inputValue = 0;
        } else {
            inputValue = 1;
        }
        inputs[p] = inputValue;
    }

    // Grab the scores and make them into fitness value by subtracting the opponents
    // score from the players score
    int playerScore = mPlayer1.mScore;
    int opponentScore = mPlayer2.mScore;
    float fitness = float(playerScore - opponentScore);
    inputs[-1] = fitness;

    return inputs;
}

std::vector<float> Game::requestPosVel(const std::vector<float>& input) {

    // Set the input
    float up = input[0];
    float down = input[1];
    if (up > 0.5) {
        this->mPlayer1.mPaddle.upPressed();
    } else {
        this->mPlayer1.mPaddle.upReleased();
    }
    if (down > 0.5) {
        this->mPlayer1.mPaddle.downPressed();
    } else {
        this->mPlayer1.mPaddle.downReleased();
    }

    // Advance the game by one frame
    progress();

    // Return game state
    b2Vec2 ballPos = mBall.mBody->GetPosition();
    b2Vec2 ballVel = mBall.mBody->GetLinearVelocity();

    b2Vec2 paddle1Pos = mPlayer1.mPaddle.mBody->GetPosition();
    b2Vec2 paddle1Vel = mPlayer1.mPaddle.mBody->GetLinearVelocity();

    b2Vec2 paddle2Pos = mPlayer2.mPaddle.mBody->GetPosition();
    b2Vec2 paddle2Vel = mPlayer2.mPaddle.mBody->GetLinearVelocity();

    int playerScore = mPlayer1.mScore;
    int opponentScore = mPlayer2.mScore;

    std::vector<float> inputs(4*2+2);
    inputs[0] = ballPos.x;
    inputs[1] = ballPos.y;
    inputs[2] = ballVel.x;
    inputs[3] = ballVel.y;
//    inputs[4] = paddle1Pos.x;
    inputs[4] = paddle1Pos.y;
//    inputs[6] = paddle1Vel.x;
    inputs[5] = paddle1Vel.y;
//    inputs[8] = paddle2Pos.x;
    inputs[6] = paddle2Pos.y;
//    inputs[10] = paddle2Vel.x;
    inputs[7] = paddle2Vel.y;
    inputs[8] = playerScore;
    inputs[9] = opponentScore;

    return inputs;
}

void Game::progress()
{
    float elapsedTime = 16;
    b2Vec2 ballVel1 = mBall.mBody->GetLinearVelocity();
    float ballSpeed1 = ballVel1.Length();

    // Run physics
    mWorld.Step(elapsedTime*mTimeStep, mVelocityIterations, mPositionIterations);

    // Ensure that ball is not slowed down, or sped up too much, and that the velocity is
    // not completely horizontal or vertical
    b2Vec2 ballVel2 = mBall.mBody->GetLinearVelocity();
    b2Vec2 newVel = ballVel2;
    float ballSpeed2 = ballVel2.Length();
    float maxIncrease = 1.5;
    if (ballSpeed2 < ballSpeed1) {
        newVel = ballSpeed1/ballSpeed2*ballVel2;
    } else if (ballSpeed2 > maxIncrease*ballSpeed1) {
        newVel = maxIncrease*ballSpeed1/ballSpeed2*ballVel2;
    }
    float minComp = 0.5;
    if (abs(newVel.x) < minComp) {
        if (newVel.x < 0) {
            newVel.x = -minComp;
        } else {
            newVel.x = minComp;
        }
    }
    if (abs(newVel.y) < minComp) {
        if (newVel.y < 0) {
            newVel.y = -minComp;
        } else {
            newVel.y = minComp;
        }
    }
    mBall.mBody->SetLinearVelocity(newVel);

    // Remove spin from the ball. We do not want to have the ball spinning, as
    // the spinning will affect the hits with the paddle.
    mBall.mBody->SetAngularVelocity(0);

    // Do not allow the paddles to go beyond the screen
    b2Vec2 paddle1Pos = mPlayer1.mPaddle.mBody->GetPosition();
    b2Vec2 paddle2Pos = mPlayer2.mPaddle.mBody->GetPosition();
    float width = mPlayer1.mPaddle.mWidth;
    float height = mPlayer1.mPaddle.mHeight;
    float maxPos = windowHeight-height/2;
    if (paddle1Pos.y > maxPos) {
        mPlayer1.mPaddle.mBody->SetTransform(b2Vec2(paddle1Pos.x, maxPos), 0);
    }
    if (paddle1Pos.y < -maxPos) {
        mPlayer1.mPaddle.mBody->SetTransform(b2Vec2(paddle1Pos.x, -maxPos), 0);
    }
    if (paddle2Pos.y > maxPos) {
        mPlayer2.mPaddle.mBody->SetTransform(b2Vec2(paddle2Pos.x, maxPos), 0);
    }
    if (paddle2Pos.y < -maxPos) {
        mPlayer2.mPaddle.mBody->SetTransform(b2Vec2(paddle2Pos.x, -maxPos), 0);
    }

    // Visualize the final state
    std::vector<GameObject*> gameObjects;
    gameObjects.push_back(&mBall);
    gameObjects.push_back(&(mPlayer1.mPaddle));
    gameObjects.push_back(&(mPlayer2.mPaddle));
    for (auto iGameObject: gameObjects) {
        iGameObject->move(elapsedTime);
        iGameObject->updateVisuals();
   }

    // Make the other paddle automatically follow the ball
    b2Vec2 diff = mPlayer2.mPaddle.mBody->GetPosition() - mBall.mBody->GetPosition();
    if (diff.y < -1.5) {
        mPlayer2.mPaddle.downReleased();
        mPlayer2.mPaddle.upPressed();
    } else if (diff.y > 1.5) {
        mPlayer2.mPaddle.upReleased();
        mPlayer2.mPaddle.downPressed();
    } else {
        mPlayer2.mPaddle.downReleased();
        mPlayer2.mPaddle.upReleased();
    }

    // If ball is outside the game area, increment score
    int ballX = mBall.mBody->GetPosition().x;
    int ballY = mBall.mBody->GetPosition().y;
    if (ballX < -windowWidth) {
        mPlayer2.score();
    } else if (ballX > windowWidth){
        mPlayer1.score();
    } else if (((ballY < -windowHeight) || (ballY > windowHeight)) && (ballX < 0)) {
        mPlayer2.score();
    } else if (((ballY < -windowHeight) || (ballY > windowHeight)) && (ballX > 0)) {
        mPlayer1.score();
    }
}
