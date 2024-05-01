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

Point LineBorder::getPoint(int index) const
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

void LineBorder::reduceEnds(int countPoints)
{
  if (this->size() >= countPoints)
  {
    toIndex = getNextIdx(toIndex, -countPoints);
    fromIndex = getNextIdx(fromIndex, countPoints);
  }
}

int LineBorder::size()
{
  if (fromIndex < toIndex)
  {
    return toIndex - fromIndex + 1;
  }
  else
  {
    return (toIndex + 1) + (owner.size() - fromIndex);
  }
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


void LineBorder::reduceEndsWhileApproxTo(LineBorder& left, LineBorder& right)
{
  Point leftPointFrom = left.getPoint(left.fromIndex);
  double leftFromRightFrom = leftPointFrom.DistanceTo(right.getPoint(right.fromIndex));

  Point leftPointTo = left.getPoint(left.toIndex);
  double leftToRightFrom = leftPointTo.DistanceTo(right.getPoint(right.fromIndex));



  int* idxLeft = &left.fromIndex;
  int stepLeft = 1;

  int* idxRight = &right.fromIndex;
  int stepRight = 1;
  
  if (leftToRightFrom < leftFromRightFrom)
  {
    idxRight = &right.toIndex;
    stepRight = -1;
  }


  for (int _ = 0; _ < 2; _++)
  {
    int newLeftIdx = left.getNextIdx(*idxLeft, stepLeft);
    int newRightIdx = right.getNextIdx(*idxRight, stepRight);
  
    double oldLeftDist = left.getPoint(*idxLeft).DistanceTo(right.getPoint(*idxRight));
    double newLeftDist = left.getPoint(newLeftIdx).DistanceTo(right.getPoint(*idxRight));

    double oldRightDist = right.getPoint(*idxRight).DistanceTo(left.getPoint(*idxLeft));
    double newRightDist = right.getPoint(newRightIdx).DistanceTo(left.getPoint(*idxLeft));

    while ((newLeftDist < oldLeftDist) || (newRightDist < oldRightDist))
    {
      if (newLeftDist < oldLeftDist)
      {
        *idxLeft = newLeftIdx;
        newLeftIdx = left.getNextIdx(*idxLeft, stepLeft);
        oldLeftDist = left.getPoint(*idxLeft).DistanceTo(right.getPoint(*idxRight));
        newLeftDist = left.getPoint(newLeftIdx).DistanceTo(right.getPoint(*idxRight));
      }

      if (newRightDist < oldRightDist)
      {
        *idxRight = newRightIdx;
        newRightIdx = left.getNextIdx(*idxRight, stepRight);
        oldRightDist = right.getPoint(*idxRight).DistanceTo(left.getPoint(*idxLeft));
        newRightDist = right.getPoint(newRightIdx).DistanceTo(left.getPoint(*idxLeft));
      }
    }

  
  
    idxLeft = &left.toIndex;
    idxRight = &right.toIndex;
  
    if (leftToRightFrom < leftFromRightFrom)
    {
      idxRight = &right.fromIndex;
    }

    stepLeft = -stepLeft;
    stepRight = -stepRight;
  }
}