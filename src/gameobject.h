#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVector2D>
#include <Box2D/Box2D.h>

class GameObject : public QObject
{
    Q_OBJECT
public:
    GameObject(b2World& world, QGraphicsScene& scene);
    GameObject(b2World& world, QGraphicsScene& scene, const QVector2D &position, const QVector2D &velocity, float mass);

    virtual void setPosition(const b2Vec2 &position);
    void setVelocity(const b2Vec2 &velocity);
    void translate(const QVector2D &translation);
    virtual void move(float dtime);
    virtual void updateVisuals();

    b2World& mWorld;
    QGraphicsScene& mScene;
    QVector2D mPosition;
    QVector2D mVelocity;
    float mMass;
    b2Body* mBody;

    float mPixelsPerMeter;

};

#endif // GAMEOBJECT_H
