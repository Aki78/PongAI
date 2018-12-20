#include "gameobject.h"
#include "constants.h"
#include <QVector2D>

GameObject::GameObject(b2World& world, QGraphicsScene& scene)
    : QObject()
    , mWorld(world)
    , mScene(scene)
    , mPosition(0, 0)
    , mVelocity(0, 0)
    , mMass(1)
    , mPixelsPerMeter(pixelsPerMeter)
{
}

GameObject::GameObject(b2World& world, QGraphicsScene& scene, const QVector2D &position, const QVector2D &velocity, float mass)
    : QObject()
    , mWorld(world)
    , mScene(scene)
    , mPosition(position)
    , mVelocity(velocity)
    , mMass(mass)
{
}

void GameObject::setPosition(const b2Vec2 &position)
{
    mBody->SetTransform(position, 0);
}

void GameObject::setVelocity(const b2Vec2 &velocity)
{
    mBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
}

void GameObject::translate(const QVector2D &dpos)
{
    mPosition += dpos;
}

void GameObject::updateVisuals()
{
}

void GameObject::move(float dtime)
{
}
