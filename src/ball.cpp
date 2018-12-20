#include "ball.h"
#include <QDebug>

Ball::Ball(b2World& world, QGraphicsScene& scene)
    : GameObject(world, scene)
    , mRadius(1)
    , mInitialSpeed(25)
    , mItem(0, 0, 2*mRadius*mPixelsPerMeter, 2*mRadius*mPixelsPerMeter)
{
    mItem.setBrush(QBrush(QColor(255, 255, 255)));
    GameObject::mScene.addItem(&mItem);

    // Setup body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    b2Body* ballBody = mWorld.CreateBody(&bodyDef);

    // Create shape
    b2CircleShape ballShape;
    ballShape.m_p.Set(0.0f, 0.0f);
    ballShape.m_radius = mRadius;

    // Create fixture
    b2FixtureDef ballFixture;
    ballFixture.shape = &ballShape;
    ballFixture.density = 1.0;
    ballFixture.friction = 0.9f;
    ballFixture.restitution = 1.0f;

    // Add fixtures to body
    ballBody->CreateFixture(&ballFixture);

    //Fix physics into ball
    mBody = ballBody;
}

void Ball::move(float dtime)
{
    mPosition += dtime*mVelocity;
}

void Ball::updateVisuals()
{
    b2Vec2 ballPosB2 = mBody->GetPosition();
    QVector2D vizPos(ballPosB2.x, -ballPosB2.y);
    vizPos *= mPixelsPerMeter;
    QVector2D offset(mRadius, mRadius);
    offset *= mPixelsPerMeter;
    vizPos -= offset;
    mItem.setPos(vizPos.toPointF());
}

bool Ball::contains(const QGraphicsItem &item) {
    return mItem.collidesWithItem(&item);
}
