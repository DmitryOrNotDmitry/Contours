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
  return owner.getNextIdx(curIndex, step);
}

Point LineBorder::getPoint(int index) const
{
  return owner.getPoint(index);
}

void LineBorder::replaceBorderWith(const LineBorder& line)
{
  Point contourPoint = getPoint(fromIndex);

  Point initPoint = line.getPoint(line.fromIndex);
  bool useBeginPoint = true;
  if (contourPoint.DistanceTo(initPoint) > contourPoint.DistanceTo(line.getPoint(line.toIndex)))
  {
    initPoint = line.getPoint(line.toIndex);
    useBeginPoint = false;
  }

  deleteContourLine();

  int countAddedPoints = 0;
  int prevSize = owner.size();

  if (contourPoint != initPoint)
  {
    std::vector<Point> connectLine = BresenhamLine::build(contourPoint, initPoint);

    for (size_t i = 0; i < connectLine.size(); i++)
    {
      owner.insertPoint(connectLine[i], fromIndex + 1 + countAddedPoints);
      countAddedPoints += owner.size() - prevSize;
      prevSize = owner.size();
    }
  }
  

  if (useBeginPoint)
  {
    for (size_t i = line.fromIndex; i != line.toIndex; i = line.getNextIdx(i, 1))
    {
      owner.insertPoint(line.getPoint(i), fromIndex + 1 + countAddedPoints);
      countAddedPoints += owner.size() - prevSize;
      prevSize = owner.size();
    }
  }
  else
  {
    for (size_t i = line.toIndex; i != line.fromIndex; i = line.getNextIdx(i, -1))
    {
      owner.insertPoint(line.getPoint(i), fromIndex + 1 + countAddedPoints);
      countAddedPoints += owner.size() - prevSize;
      prevSize = owner.size();
    }
  }

  if (fromIndex <= toIndex)
    contourPoint = getPoint(toIndex + countAddedPoints);
  else
    contourPoint = getPoint(toIndex);

  initPoint = line.getPoint(line.toIndex);
  if (!useBeginPoint)
    initPoint = line.getPoint(line.fromIndex);

  if (contourPoint != initPoint)
  {
    std::vector<Point> connectLine = BresenhamLine::build(initPoint, contourPoint);

    for (size_t i = 0; i < connectLine.size(); i++)
    {
      owner.insertPoint(connectLine[i], fromIndex + 1 + countAddedPoints);
      countAddedPoints += owner.size() - prevSize;
      prevSize = owner.size();
    }
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

int LineBorder::size() const
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