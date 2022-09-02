#include "stdafx.h"
#include "point.h"

Point Point::getPoints() const { return { px, py }; }

Point Point::getDiff(const Point& other) const {
    return { std::max(0, px - other.px), std::max(0, py - other.py) };
}

bool Point::isStrictryInside(const Point& bottomLeft, const Point& topRight) const {
  return bottomLeft.px < px&& px < topRight.px&& bottomLeft.py < py&& py < topRight.py;
}

bool Point::isOnBoundary(const Point& bottomLeft, const Point& topRight) const {
  return (bottomLeft.px == px && bottomLeft.py <= py && py <= topRight.py)
    || (topRight.px == px && bottomLeft.py <= py && py <= topRight.py)
    || (bottomLeft.py == py && bottomLeft.px <= px && px <= topRight.px)
    || (topRight.py == py && bottomLeft.px <= px && px <= topRight.px);
}

bool Point::isInside(const Point& bottomLeft, const Point& topRight) const {
  return isStrictryInside(bottomLeft, topRight) || isOnBoundary(bottomLeft, topRight);
}

int Point::getScalarSize() const { return px * py; }

Point Point::add(const Point& otherPoint) const {
  return Point(px + otherPoint.px, py + otherPoint.py);
}

Point Point::subtract(const Point& otherPoint) const {
  return Point(px - otherPoint.px, py - otherPoint.py);
}
