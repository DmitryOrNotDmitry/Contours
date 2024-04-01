#include "StdAfx.h"
#include "Contour.h"


Contour::Contour()
{
}


Contour::~Contour()
{
}


void Contour::addPoint(Point point)
{
  points.push_back(point);
}


void Contour::addPoint(int x, int y)
{
  points.push_back(Point(x, y));
}


std::vector<Point> Contour::addPoints(std::vector<Point>& newPoints)
{
  std::vector<Point> fitPoints;
  
  if (newPoints.size() == 0)
    return fitPoints;

  if (points.size() > 0 && newPoints.size() > 1)
  {
    if ((*points.rbegin()).DistanceTo(*newPoints.begin()) > (*points.rbegin()).DistanceTo(*newPoints.rbegin()))
      std::reverse(newPoints.begin(), newPoints.end());
  }

  size_t numNewPoints = newPoints.size();
  if (points.size() < numNewPoints)
    numNewPoints = points.size();
  fitPoints.reserve(numNewPoints);
  
  for (auto iter = newPoints.begin(); iter != newPoints.end(); iter++)
  {
    if (*iter == Point(206, 188))
      int t = 0;

    bool isUniquePoint = true;
    for (size_t i = 0; i < numNewPoints; i++)
    {
      if (*iter == *(points.rbegin() + i))
      {
        isUniquePoint = false;
        break;
      }
    }

    if (isUniquePoint)
    {
      if (fitPoints.size() > 0)
      {
        if ((*fitPoints.rbegin()).DistanceTo(*iter) < 2)
          fitPoints.push_back(*iter);
      }
      else if (points.size() > 0)
      {
        if ((*points.rbegin()).DistanceTo(*iter) < 2)
          fitPoints.push_back(*iter);
      }
      else
      {
        fitPoints.push_back(*iter);
      }
    }

  }

  points.insert(points.cend(), fitPoints.begin(), fitPoints.end());

  if (points.size() > 1)
    while (*points.begin() == *points.rbegin())
      points.erase(--points.end());

  return fitPoints;
}

std::vector<Point>& Contour::getPoints()
{
  return points;
}

Point Contour::getLastPoint()
{
  return *points.rbegin();
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

Point Contour::operator[](int i) const
{
  return points[i];
}

bool Contour::isEmpty() const
{
  return points.empty();
}

int Contour::findNearestPointTo(const Point& destination, int from, int to, int step) const
{
  double minDist = 1e20;
  int minItemIndex = 0;

  int realindex;
  for (int i = from; i < to; i += step)
  {
    if (i >= (int)size() || i < 0)
      realindex = i + size();
    realindex = i % size();

    double curDist = points[realindex].DistanceTo(destination);
    if (curDist < minDist)
    {
      minDist = curDist;
      minItemIndex = realindex;
    }
  }

  return minItemIndex;
}

int Contour::findNearestPointTo(const Point& destination, int step) const
{
  return findNearestPointTo(destination, 0, points.size(), step);
}
