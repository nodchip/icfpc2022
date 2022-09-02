#pragma once

struct Point
{
  int px;
  int py;

  Point(int px = 0, int py = 0) : px(px), py(py) {}

  Point getPoints() const;

  Point getDiff(const Point& other) const;

  bool isStrictryInside(const Point& bottomLeft, const Point& topRight) const;

  bool isOnBoundary(const Point& bottomLeft, const Point& topRight) const;

  bool isInside(const Point& bottomLeft, const Point& topRight) const;

  int getScalarSize() const;

};
