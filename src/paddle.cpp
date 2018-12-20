#include "paddle.h"
#include <QDebug>
#include <QPen>

Paddle::Paddle(b2World& world, QGraphicsScene& scene)
    : GameObject(world, scene)
    , mWidth(2)
    , mHeight(6)
    , mSpeed(60)
    , mGoingUp(false)
    , mGoingDown(false)
    , mItem(0, 0, 1.0*mWidth*mPixelsPerMeter, 1.0*mHeight*mPixelsPerMeter)
{
    // Setup body
    b2BodyDef bodyDef;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position.Set(0.0f, 0.0f);
    b2Body* paddleBody = mWorld.CreateBody(&bodyDef);

    // Create shape
    b2PolygonShape paddleShape;
    paddleShape.SetAsBox(mWidth/2, mHeight/2, b2Vec2(0, 0), 0);

    // Create fixture
    b2FixtureDef paddleFixture;
    paddleFixture.shape = &paddleShape;
    paddleFixture.density = 1.0f;
    paddleFixture.friction = 0.9;
    paddleFixture.restitution = 1.0f;
    paddleFixture.userData = this;

    // Add fixture to body
    paddleBody->CreateFixture(&paddleFixture);

    mBody = paddleBody;

    // Create the visual lines
    mItem.setBrush(QBrush(QColor(255, 255, 255)));
    mScene.addItem(&mItem);
}

void Paddle::move(float dtime)
{
   b2Vec2 vel = b2Vec2(0, 0);
   float speed = mSpeed;
   if (mGoingDown & !mGoingUp) {
       vel += b2Vec2(0, -speed);
   }
   if (mGoingUp && !mGoingDown) {
       vel += b2Vec2(0, speed);
   }
   mBody->SetLinearVelocity(vel);
}

void Paddle::updateVisuals()
{
    b2Vec2 pos = mBody->GetPosition();
    QVector2D vizPos(pos.x, -pos.y);
    QVector2D offset(-mWidth/2, -mHeight/2);
    vizPos += offset;
    vizPos *= mPixelsPerMeter;
    mItem.setPos(vizPos.toPointF());
}

void Paddle::upPressed()
{
    mGoingUp = true;
}
void Paddle::downPressed()
{
    mGoingDown = true;
}
void Paddle::upReleased()
{
    mGoingUp = false;
}
void Paddle::downReleased()
{
    mGoingDown = false;
}
void Paddle::rightPressed()
{
    mGoingRight = true;
}
void Paddle::leftPressed()
{
    mGoingLeft = true;
}
void Paddle::rightReleased()
{
    mGoingRight = false;
}
void Paddle::leftReleased()
{
    mGoingLeft = false;
}
