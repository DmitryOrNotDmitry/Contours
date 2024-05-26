#include "LineBorderVector.h"

LineBorderVector::LineBorderVector(const std::vector<Point>& points)
  : LineBorder(Contour(), 0, points.size() - 1)
  , points(points)
{

}

int LineBorderVector::getNextIdx(int curIndex, int step) const
{
  return (curIndex + step) % size();
}

Point LineBorderVector::getPoint(int index) const
{
  return points[index];
}

int LineBorderVector::size() const
{
  return points.size();
}
