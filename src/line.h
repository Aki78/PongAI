#ifndef LINE_H
#define LINE_H

#include <QVector2D>

class Line
{
public:
    Line();
    Line(const QVector2D &start, const QVector2D &end);
    QVector2D mStart;
    QVector2D mEnd;
    QVector2D getNormal() const;
    QVector2D getVector() const;
    QVector2D getClosestPoint(const QVector2D &point) const;

};

#endif // LINE_H
