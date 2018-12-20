#ifndef PADDLE_H
#define PADDLE_H

#include "gameobject.h"
#include "line.h"
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QObject>


class Paddle : public GameObject
{
    Q_OBJECT

public:
    Paddle(b2World& world, QGraphicsScene& scene);

    float mWidth;
    float mHeight;
    float mSpeed;
    bool mGoingUp;
    bool mGoingDown;
    bool mGoingRight;
    bool mGoingLeft;
    QGraphicsRectItem mItem;

    void move(float dtime) override;
    void updateVisuals() override;

public slots:
    void upPressed();
    void downPressed();
    void upReleased();
    void downReleased();
    void rightPressed();
    void leftPressed();
    void rightReleased();
    void leftReleased();
};
#endif // PADDLE_H
