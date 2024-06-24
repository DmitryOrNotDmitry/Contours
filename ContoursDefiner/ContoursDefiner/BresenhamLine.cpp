#include "BresenhamLine.h"
#include <cmath>


void BresenhamLine::build(const Point& pointFrom, const Point& pointTo)
{
  int posibleSize = abs(pointFrom.y - pointTo.y);
  if (posibleSize < abs(pointFrom.x - pointTo.x))
    posibleSize = abs(pointFrom.x - pointTo.x);

  line.reserve(posibleSize);

  double x0 = pointFrom.x + 0.5;
  double y0 = pointFrom.y + 0.5;
  double x1 = pointTo.x + 0.5;
  double y1 = pointTo.y + 0.5;
  
  bool changeCoords = false;

  if (abs(pointTo.y - pointFrom.y) > abs(pointTo.x - pointFrom.x))
  {
    std::swap(x0, y0);
    std::swap(x1, y1);
    changeCoords = true;
  }

  const double factor = (y1 - y0) / (x1 - x0);

  double deltaX = 0.1;
  if ((x1 - x0) < 0)
    deltaX = -deltaX;

  double x = x0;
  double y = 0;
  Point curPoint;
  line.push_back(pointFrom);

  while (abs(x1 - x) > abs(deltaX))
  {
    y = factor * (x - x0) + y0;

    if (changeCoords)
      curPoint = Point(y, x);
    else
      curPoint = Point(x, y);

    
    if (*line.rbegin() != curPoint)
      line.push_back(curPoint);

    x += deltaX;
  }

}

BresenhamLine::BresenhamLine(const Point& pointFrom, const Point& pointTo)
{
  build(pointFrom, pointTo);
}

Point BresenhamLine::operator[](int i) const
{
  return line[i];
}

size_t BresenhamLine::size() const
{
  return line.size();
}

bool BresenhamLine::operator==(const std::vector<Point>& vector) const
{
  return vector == line;
}