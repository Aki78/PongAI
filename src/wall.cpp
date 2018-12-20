#include "wall.h"

Wall::Wall(b2World& world, QGraphicsScene& scene, const QVector2D &start, const QVector2D &end)
    : GameObject(world, scene)
{
    b2Vec2 v0(start.x(), start.y());
    b2Vec2 v1(end.x(), end.y());

    // Create body
    b2BodyDef wallBodyDef;
    wallBodyDef.position.Set(0.0f, 0.0f);
    b2Body* wallBody = world.CreateBody(&wallBodyDef);

    // Create shape
    b2EdgeShape wallShape;
    wallShape.Set(v0, v1);

    // Create fixture
    b2FixtureDef wallFixture;
    wallFixture.shape = &wallShape;
    wallFixture.density = 0.0f;
    wallFixture.friction = 0.0f;
    wallFixture.restitution = 1.0f;
    wallBody->CreateFixture(&wallFixture);

    // Create visuals
//    QPen linePen(QColor(255, 255, 255));
//    QVector2D visStart = QVector2D(start);
//    QVector2D visEnd = QVector2D(end);
//    visStart *= mPixelsPerMeter;
//    visEnd *= mPixelsPerMeter;
//    visStart.setY(-visStart.y());
//    visEnd.setY(-visEnd.y());
//    QLineF lineF(visStart.toPointF(), visEnd.toPointF());
//    QGraphicsLineItem* vLine = new QGraphicsLineItem(lineF);
//    vLine->setPen(linePen);
//    mScene.addItem(vLine);
}
