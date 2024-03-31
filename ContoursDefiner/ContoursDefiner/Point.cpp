#include "Point.h"
#include <cmath>
#include <utility>

Point::Point(int x, int y)
  : x(x)
  , y(y)
{
}


Point::Point()
{
}

Point::Point(const Point& other)
{
  *this = other;
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

double Point::DistanceTo(const Point& to) const
{
  return sqrt(pow((double)this->x - to.x, 2) + pow((double)this->y - to.y, 2));
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
  if (this->y < other.y)
    return true;
  
  if (this->y == other.y)
    return this->x < other.x;

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

Point& Point::operator=(Point&& other)
{
  x = std::move(other.x);
  y = std::move(other.y);

  return *this;
}
