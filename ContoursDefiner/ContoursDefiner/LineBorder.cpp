#include "LineBorder.h"

#define CONNECT_LINE_MIN_DISTANCE 1.999

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

int LineBorder::getFromIndex() const
{
  return fromIndex;
}

int LineBorder::getToIndex() const
{
  return toIndex;
}

Point LineBorder::fromPoint() const
{
  return getPoint(getFromIndex());
}

Point LineBorder::toPoint() const
{
  return getPoint(getToIndex());
}

int LineBorder::getNextIdx(int curIndex, int step) const
{
  return owner.getNextIdx(curIndex, step);
}

Point LineBorder::getPoint(int index) const
{
  return owner.getPoint(index);
}

void LineBorder::insertLine(const LineBorder& line, int startIdx, int step)
{
  int lineSize = line.size();
  int idx = startIdx;

  int countInserted = 0;
  int prevSize = owner.size();

  for (int i = 0; i < lineSize; i++)
  {
    owner.insertPoint(line.getPoint(idx), fromIndex + 1 + countInserted);
    countInserted += owner.size() - prevSize;
    prevSize = owner.size();

    idx = line.getNextIdx(idx, step);
  }
}

void LineBorder::replaceBorderWith(const LineBorder& line)
{
  Point contourPoint = getPoint(fromIndex);

  Point initPoint = line.fromPoint();
  bool useBeginPoint = true;
  if (contourPoint.DistanceTo(initPoint) > contourPoint.DistanceTo(line.toPoint()))
  {
    initPoint = line.toPoint();
    useBeginPoint = false;
  }

  deleteContourLine();

  int countAddedPoints = 0;
  int prevSize = owner.size();

  if (contourPoint.DistanceTo(initPoint) >= CONNECT_LINE_MIN_DISTANCE)
  {
    BresenhamLine connectLine(contourPoint, initPoint);

    for (size_t i = 0; i < connectLine.size(); i++)
    {
      owner.insertPoint(connectLine[i], fromIndex + 1 + countAddedPoints);
      countAddedPoints += owner.size() - prevSize;
      prevSize = owner.size();
    }
  }
  
  if (useBeginPoint)
  {
    insertLine(line, line.fromIndex, 1);
  }
  else
  {
    insertLine(line, line.toIndex, -1);
  }

  countAddedPoints += owner.size() - prevSize;
  prevSize = owner.size();

  if (fromIndex <= toIndex)
    contourPoint = getPoint(toIndex + countAddedPoints);
  else
    contourPoint = getPoint(toIndex);

  initPoint = line.toPoint();
  if (!useBeginPoint)
    initPoint = line.fromPoint();

  if (contourPoint.DistanceTo(initPoint) >= CONNECT_LINE_MIN_DISTANCE)
  {
    BresenhamLine connectLine(contourPoint, initPoint);

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
  if (fromIndex <= toIndex)
  {
    return toIndex - fromIndex + 1;
  }
  else
  {
    return (toIndex + 1) + (owner.size() - fromIndex);
  }
}

double LineBorder::euclideanLength() const
{
  double len = 0;
  int idx = fromIndex;
  int nextIdx = getNextIdx(idx, 1);
  
  for (int i = 0; i < size() - 1; i++)
  {
    len += getPoint(idx).DistanceTo(getPoint(nextIdx));

    idx = getNextIdx(idx, 1);
    nextIdx = getNextIdx(nextIdx, 1);
  }

  return len;
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
  else if (fromIndex != toIndex)
  {
    points.erase(points.begin() + fromIndex + 1, points.begin() + toIndex);
    toIndex -= (toIndex - fromIndex - 1);
  }
}


void LineBorder::reduceEndsWhileApproxTo(LineBorder& left, LineBorder& right, int maxDeleted)
{
  if (left.size() < maxDeleted || right.size() < maxDeleted)
    maxDeleted = left.size();

  if (right.size() < maxDeleted)
    maxDeleted = right.size();

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
    if (maxDeleted > left.size())
      maxDeleted = left.size();

    if (maxDeleted > right.size())
      maxDeleted = right.size();

    int startIndex = *idxLeft;
    double minDist = right.owner.distanceTo(left.getPoint(startIndex));

    for (int i = 1; i < maxDeleted; i++)
    {
      int nextIdx = left.getNextIdx(startIndex, i * stepLeft);
      double curDist = right.owner.distanceTo(left.getPoint(nextIdx));
      if (curDist < minDist)
      {
        minDist = curDist;
        *idxLeft = nextIdx;
      }
    }
    
    startIndex = *idxRight;
    minDist = left.owner.distanceTo(right.getPoint(startIndex));

    for (int i = 1; i < maxDeleted; i++)
    {
      int nextIdx = right.getNextIdx(startIndex, i * stepRight);
      double curDist = left.owner.distanceTo(right.getPoint(nextIdx));
      if (curDist < minDist)
      {
        minDist = curDist;
        *idxRight = nextIdx;
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

LineBorder LineBorder::inverse() const
{
  return LineBorder(owner, toIndex, fromIndex);
}

void LineBorder::agreeWith(const LineBorder& line)
{
  int offset = 0;
  int idx = fromIndex;
  int idxOther = line.fromIndex;
  for (int i = 0; i < size(); i++)
  {
    if (getPoint(idx) == line.getPoint(idxOther))
      break;
    
    idxOther = line.getNextIdx(idxOther);
    offset++;
  }

  if (offset > 0)
  {
    fromIndex = getNextIdx(fromIndex, -offset);
    toIndex = getNextIdx(toIndex, -offset);
  }
}

bool LineBorder::isClockwise() const
{
  return owner.isClockwise(fromIndex, toIndex);
}
