#include "LineBorderVector.h"
#include <algorithm>

LineBorderVector::LineBorderVector(const std::vector<Point>& points)
  : LineBorder( Contour(), 0, std::max(0, static_cast<int>(points.size() - 1) ))
  , points(points)
{

}

LineBorderVector& LineBorderVector::operator=(const LineBorderVector& other)
{
  this->points = other.points;

  return *this;
}

int LineBorderVector::getNextIdx(int curIndex, int step) const
{
  int idx = curIndex + step;
  if (idx < 0)
    idx += size();

  return idx % size();
}

Point LineBorderVector::getPoint(int index) const
{
  return points[index];
}

int LineBorderVector::size() const
{
  return points.size();
}

