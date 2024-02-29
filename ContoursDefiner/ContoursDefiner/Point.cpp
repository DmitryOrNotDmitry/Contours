//#include "StdAfx.h"
#include "Point.h"


Point::Point(int x, int y)
  : x(x)
  , y(y)
{
}


Point::Point()
{
}


Point::~Point(void)
{
}

Point Point::toRight(int offset) const
{
  Point newPoint(*this);

  newPoint.x += offset;

  return newPoint;
}

Point Point::toLeft(int offset) const
{
  Point newPoint(*this);

  newPoint.x -= offset;

  return newPoint;
}

Point Point::toBottom(int offset) const
{
  Point newPoint(*this);

  newPoint.y += offset;

  return newPoint;
}

Point Point::toUp(int offset) const
{
  Point newPoint(*this);

  newPoint.y -= offset;

  return newPoint;
}

bool Point::operator==(const Point& other) const
{
  return this->x == other.x && this->y == other.y;
}

bool Point::operator!=(const Point& other) const
{
  return !(*this == other);
}

Point& Point::operator=(const Point& other)
{
  if (this == &other)
      return *this;
  
  this->x = other.x;
  this->y = other.y;

  return *this;
}
  