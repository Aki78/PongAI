#ifndef WALL_H
#define WALL_H

#include "gameobject.h"
#include "line.h"


class Wall : public GameObject
{
public:
    Wall(b2World& world, QGraphicsScene& scene, const QVector2D &start, const QVector2D &end);
};

#endif // WALL_H
