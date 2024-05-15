#include "StdAfx.h"
#include "Contour.h"
#include "Path.h"


Contour::Contour()
{
  lastKAddedPoints.resize(K, Point(-1, -1));
}


Contour::~Contour()
{
}


void Contour::addPoint(Point point)
{
  if (points.size() > 0 && (*points.rbegin() == point))
    return;
  
  points.push_back(point);
}

void Contour::insertPoint(Point point, int index)
{
  if (index > 0 && (*(points.begin() + index - 1) == point))
    return;

  if (index < points.size() && (*(points.begin() + index) == point))
    return;

  if (index == points.size() && points.size() > 1 && (points[0] == point))
    return;

  points.insert(points.begin() + index, point);
}


void Contour::addPoint(int x, int y)
{
  addPoint(Point(x, y));
}


std::vector<Point> Contour::addPoints(std::vector<Point>& newPoints)
{
  std::vector<Point> fitPoints;

  if (newPoints.size() == 0)
    return fitPoints;

  deleteYetAddedPoints(newPoints);

  Path path(newPoints);

  if (points.size() > 0 && path.size() > 1)
  {
    if ((*points.rbegin()).DistanceTo(path[0]) > (*points.rbegin()).DistanceTo(path[-1]))
      path.reverse();
  }

  if (points.size() > 1 && points.size() < 8 && path.size() > 0)
  {
    if (path[0].DistanceTo(*points.begin()) < path[0].DistanceTo(*points.rbegin()))
      std::reverse(points.begin(), points.end());
  }
  
  
  for (auto iter = path.begin(); iter != path.end(); iter++)
  {
    double distance = 0;

    if (fitPoints.size() > 0)
    {
      distance = fitPoints.rbegin()->DistanceTo(*iter);
      if (distance < 2)
        fitPoints.push_back(*iter);
    }
    else if (points.size() > 0)
    {
      distance = points.rbegin()->DistanceTo(*iter);
      if (distance < 2)
        fitPoints.push_back(*iter);
    }
    else
    {
      fitPoints.push_back(*iter);
    }

  }

  points.insert(points.cend(), fitPoints.begin(), fitPoints.end());

  std::vector<Point> tmp(fitPoints);

  memoryLastAddedPoints(std::move(tmp));

  return fitPoints;
}


std::vector<Point>& Contour::getPoints()
{
  return points;
}


ContourState Contour::getState() const
{
  return state;
}


void Contour::setState(ContourState state)
{
  this->state = state;
}


size_t Contour::size() const
{
  return points.size();
}


Point* Contour::getData()
{
  return points.data();
}

bool Contour::operator==(const Contour& other) const
{
    return this->points == other.points;
}

bool Contour::operator!=(const Contour& other) const
{
  return !(*this == other);
}

Point& Contour::operator[](int i)
{
  return points[i];
}

const Point& Contour::operator[](int i) const
{
  return points[i];
}

bool Contour::isEmpty() const
{
  return points.empty();
}

int Contour::findNearestPointTo(const Point& destination, int from, int count, int step) const
{
  double minDist = 10e100;
  int minItemIndex = 0;

  int realIndex = from;

  for (int i = 0; i < count; i++)
  {
    double curDist = points[realIndex].DistanceTo(destination);
    if (curDist < minDist)
    {
      minDist = curDist;
      minItemIndex = realIndex;
    }
    realIndex = getNextIdx(realIndex, step);
  }

  return minItemIndex;
}


int Contour::findNearestPointTo(const Point& destination, int step) const
{
  return findNearestPointTo(destination, 0, points.size(), step);
}


void Contour::removeSamePointsAtEnds()
{
  if (points.size() == 0)
    return;

  Point& lastPoint = *points.rbegin();
  
  int sameIdx = -1;
  for (int i = 0; i < 8; i++)
  {
    if (points[i] == lastPoint)
    {
      sameIdx = i;
      break;
    }
  }

  if (sameIdx == -1)
    return;

  bool isEqualsPointChain = true;
  for (int i = 0; i < sameIdx; i++)
  {
    if (*(points.rbegin() + i) != points[sameIdx - i])
    {
      isEqualsPointChain = false;
      break;
    }
  }

  if (isEqualsPointChain)
  {
    points.erase(points.end() - (sameIdx + 1), points.end());
  }

}


void Contour::memoryLastAddedPoints(std::vector<Point>&& points)
{
  if (points.size() == 0)
    points.push_back(Point(-1, -1));

  for (size_t j = 0; j < points.size(); j++)
  {
    for (size_t i = K - 1; i > 0; i--)
    {
      lastKAddedPoints[i] = std::move(lastKAddedPoints[i - 1]);
    }
    if (K > 0)
      lastKAddedPoints[0] = std::move(points[j]);
  }
}


void Contour::deleteYetAddedPoints(std::vector<Point>& deletedPoints)
{
  for (size_t i = 0; i < K; i++)
  {
    auto foundedPoint = std::find(deletedPoints.begin(), deletedPoints.end(), lastKAddedPoints[i]);
    if (foundedPoint != deletedPoints.end())
      deletedPoints.erase(foundedPoint);
  }
}

bool Contour::haveRepeatPoints() const
{
  bool haveRepeat = false;
  for (size_t i = 1; i < size(); i++)
  {
    if (points[i] == points[i - 1])
    {
      haveRepeat = true;
    }
  }
  if (points.size() > 1 && (*points.rbegin() == points[0]))
    haveRepeat = true;

  return haveRepeat;
}

Point Contour::getPoint(int index) const
{
  return points[index];
}

int Contour::getNextIdx(int fromIndex, int step) const
{
  int size = this->size();
  int nextIndex = fromIndex + step;

  if (nextIndex < 0)
    nextIndex += size;

  if (nextIndex > size - 1)
  {
    nextIndex %= size;
  }

  return nextIndex;
}


double Contour::distanceTo(const Point& destination) const
{
  int nearestPointIdx = findNearestPointTo(destination);
  return destination.DistanceTo(getPoint(nearestPointIdx));
}
