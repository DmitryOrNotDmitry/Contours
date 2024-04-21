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


void LineBorder::replaceLine(const std::vector<Point>& averagePoints)
{
  if (averagePoints.size() == 0)
    return;

  Point contourPoint = getPoint(fromIndex);

  Point averagePoint = *averagePoints.begin();
  bool useBeginPoint = true;
  if (contourPoint.DistanceTo(averagePoint) > contourPoint.DistanceTo(*averagePoints.rbegin()))
  {
    averagePoint = *averagePoints.rbegin();
    useBeginPoint = false;
  }

  std::vector<Point> connectLine = BresenhamLine::build(contourPoint, averagePoint);

  deleteContourLine();

  int countAddedPoints = 0;
  int prevSize = owner.size();
  for (size_t i = 0; i < connectLine.size(); i++)
  {
    owner.insertPoint(connectLine[i], fromIndex + 1 + countAddedPoints);
    countAddedPoints += owner.size() - prevSize;
    prevSize = owner.size();
  }

  if (useBeginPoint)
  {
    for (size_t i = 0; i < averagePoints.size(); i++)
    {
      owner.insertPoint(averagePoints[i], fromIndex + 1 + countAddedPoints);
      countAddedPoints += owner.size() - prevSize;
      prevSize = owner.size();
    }
  }
  else
  {
    for (size_t i = 0; i < averagePoints.size(); i++)
    {
      owner.insertPoint(averagePoints[averagePoints.size() - i - 1], fromIndex + 1 + countAddedPoints);
      countAddedPoints += owner.size() - prevSize;
      prevSize = owner.size();
    }
  }

  if (fromIndex <= toIndex)
    contourPoint = getPoint(toIndex + countAddedPoints);
  else
    contourPoint = getPoint(toIndex);

  averagePoint = *averagePoints.rbegin();
  if (!useBeginPoint)
    averagePoint = *averagePoints.begin();

  connectLine = BresenhamLine::build(averagePoint, contourPoint);

  for (size_t i = 0; i < connectLine.size(); i++)
  {
    owner.insertPoint(connectLine[i], fromIndex + 1 + countAddedPoints);
    countAddedPoints += owner.size() - prevSize;
    prevSize = owner.size();
  }

  if (fromIndex <= toIndex)
    toIndex += countAddedPoints;
}

void LineBorder::deleteContourLine()
{
  std::vector<Point>& points = owner.getPoints();
  if (fromIndex > toIndex)
  {
    points.erase(points.begin() + fromIndex + 1, points.end());
    points.erase(points.begin(), points.begin() + toIndex);
    fromIndex = points.size() - 1;
    toIndex = 0;
  }
  else 
  {
    points.erase(points.begin() + fromIndex + 1, points.begin() + toIndex);
    toIndex -= (toIndex - fromIndex - 1);
  }
}
