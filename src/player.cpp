#include "player.h"
#include "game.h"
#include "constants.h"

Player::Player(Game& game, bool manual)
    : mScore(0)
    , mPaddle(game.mWorld, game.mScene)
    , mGame(game)
    , mManual(manual)
    , mScoreText("0")
{
    QColor scoreColor = QColor(255, 255, 255);
    QFont scoreFont = QFont("ArcadeClassic", pixelsPerMeter*5);
    mScoreText.setDefaultTextColor(scoreColor);
    mScoreText.setFont(scoreFont);
    positionScoreText();
    mGame.mScene.addItem(&mScoreText);
}

void Player::score()
{
    ++mScore;
    mScoreText.setPlainText(QString::number(mScore));
    positionScoreText();
    float randY = 20*(float(rand())/float(RAND_MAX)-0.5);
    float factor = mManual ? -1 : 1;
    mGame.mBall.setPosition(b2Vec2(0, 0));
    mGame.mBall.setVelocity(b2Vec2(factor*mGame.mBall.mInitialSpeed, randY));
}

void Player::positionScoreText() {
    float textWidth = mScoreText.boundingRect().width();
    float factor = mManual ? -1 : 1;
    mScoreText.setPos(factor*0.2*windowWidth*pixelsPerMeter-textWidth/2, -0.9*windowHeight*pixelsPerMeter);
}

void Player::reset()
{
}
