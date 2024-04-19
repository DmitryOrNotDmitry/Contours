#include "LineBorder.h"


LineBorder::LineBorder(Contour& owner, int fromIndex, int toIndex)
  : owner(owner)
  , fromIndex(fromIndex)
  , toIndex(toIndex)
{
}

LineBorder& LineBorder::operator=(const LineBorder& other)
{
  this->owner = other.owner;
  this->fromIndex = other.fromIndex;
  this->toIndex = other.toIndex;

  return *this;
}

Contour& LineBorder::getOwner()
{
  return owner;
}

int LineBorder::getFromIndex()
{
  return fromIndex;
}

int LineBorder::getToIndex()
{
  return toIndex;
}

int LineBorder::getNextIdx(int curIndex, int step) const
{
  int size = owner.size();
  curIndex += step;

  if (curIndex < 0)
    curIndex += size;

  if (curIndex > size - 1)
  {
    curIndex %= size;
  }

  return curIndex;
}

Point LineBorder::getPoint(int index)
{
  std::vector<Point>& points = owner.getPoints();
  if (index < 0)
    index = points.size() + index;

  if (index < 0 || index > points.size())
    return Point(-1, -1);

  return points[index];
}
