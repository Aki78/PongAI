#ifndef BALL_H
#define BALL_H
#include "gameobject.h"
#include <QVector2D>

class Ball : public GameObject
{
public:
    Ball(b2World& world, QGraphicsScene& scene);
    float mRadius;
    float mInitialSpeed;
    QGraphicsEllipseItem mItem;

    void move(float dtime) override;
    void updateVisuals() override;

    bool contains(const QGraphicsItem &item);
};

#endif // BALL_H
