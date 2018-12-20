#include "line.h"
#include <algorithm>

Line::Line()
    : mStart(0, 0)
    , mEnd(0, 0)
{
}

Line::Line(const QVector2D &start, const QVector2D &end)
    : mStart(start)
    , mEnd(end)
{
}

QVector2D Line::getNormal() const
{
    QVector2D line = mEnd-mStart;
    line.normalize();
    QVector2D normal(line.y(), -line.x());
    return normal;
}

QVector2D Line::getVector() const
{
    QVector2D line = mEnd-mStart;
    return line;
}

QVector2D Line::getClosestPoint(const QVector2D &point) const
{
    QVector2D line = mEnd-mStart;
    const float l2 = line.lengthSquared();  // i.e. |w-v|^2 -  avoid a sqrt

    // Case where start == end
    if (l2 == 0.0) {
        return mStart;
    }

    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    const float t = std::max(0.0f, std::min(1.0f, QVector2D::dotProduct(point - mStart, mEnd - mStart) / l2));
    const QVector2D projection = mStart + t * (mEnd - mStart);  // Projection falls on the segment
    return projection;
}
