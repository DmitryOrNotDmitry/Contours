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

double Point::DistanceTo(const Point& to)
{
  return sqrt(pow(this->x - to.x, 2) + pow(this->y - to.y, 2));
}

bool Point::operator==(const Point& other) const
{
  return this->x == other.x && this->y == other.y;
}

bool Point::operator!=(const Point& other) const
{
  return !(*this == other);
}

bool Point::operator<(const Point& other) const
{
  if (this->x < other.x)
    return true;
  
  if (this->x == other.x)
    return this->y < other.y;

  return false;
}

Point& Point::operator=(const Point& other)
{
  if (this == &other)
      return *this;
  
  this->x = other.x;
  this->y = other.y;

  return *this;
}

Point::operator POINT() const
{
  POINT point;
  point.x = x;
  point.y = y;
  return point;
}
  