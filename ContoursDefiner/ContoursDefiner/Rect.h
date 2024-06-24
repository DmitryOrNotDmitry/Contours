#pragma once
#include "Point.h"
#include <algorithm>

class Rect
{
  Point leftUp;
  Point rightBottom;

public:

  static Rect nullRect;

  Rect();
  Rect(Point leftUp, Point rightBottom);

  Rect intersect(const Rect& other) const;
  bool isInner(const Point& point) const;

  bool operator==(const Rect& other) const;

};

