#include "Rect.h"

#include <algorithm>

Rect Rect::nullRect = Rect(Point(-1, -1), Point(-1, -1));

Rect::Rect()
{
  *this = Rect::nullRect;
}

Rect::Rect(Point leftUp, Point rightBottom)
  : leftUp(leftUp)
  , rightBottom(rightBottom)
{
}

Rect Rect::intersect(const Rect& other) const
{
  int xLeft = std::max(leftUp.x, other.leftUp.x);
  int yLeft = std::max(leftUp.y, other.leftUp.y);
  
  int xRight = std::min(rightBottom.x, other.rightBottom.x);
  int yRight = std::min(rightBottom.y, other.rightBottom.y);

  Rect intersectRect(Point(xLeft, yLeft), Point(xRight, yRight));

  if ((intersectRect.leftUp.x > intersectRect.rightBottom.x) || (intersectRect.leftUp.y > intersectRect.rightBottom.y))
    intersectRect = Rect::nullRect;

  return intersectRect;
}

bool Rect::isInner(const Point& point) const
{
  return (point.x >= leftUp.x) && (point.x <= rightBottom.x) && (point.y >= leftUp.y) && (point.y <= rightBottom.y);
}

bool Rect::operator==(const Rect& other) const
{
  return (leftUp == other.leftUp) && (rightBottom == other.rightBottom);
}
