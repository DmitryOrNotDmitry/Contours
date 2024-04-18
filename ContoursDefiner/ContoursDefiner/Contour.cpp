#include "StdAfx.h"
#include "Contour.h"


Contour::Contour()
{
  lastKAddedPoints.resize(K, Point(-1, -1));
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


  deleteYetAddedPoints(newPoints);

  if (points.size() > 0 && newPoints.size() > 1)
  {
    if ((*points.rbegin()).DistanceTo(*newPoints.begin()) > (*points.rbegin()).DistanceTo(*newPoints.rbegin()))
      std::reverse(newPoints.begin(), newPoints.end());
  }

  if (points.size() > 1 && points.size() < 8 && newPoints.size() > 0)
  {
    if (newPoints.begin()->DistanceTo(*points.begin()) < newPoints.begin()->DistanceTo(*points.rbegin()))
      std::reverse(points.begin(), points.end());
  }
  
  
  for (auto iter = newPoints.begin(); iter != newPoints.end(); iter++)
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

  /*if (points.size() > 1)
    while (*points.begin() == *points.rbegin())
      points.erase(--points.end());*/

  
  std::vector<Point> qwe(fitPoints);

  memoryLastAddedPoints(std::move(qwe));

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


void Contour::memoryLastAddedPoints(std::vector<Point>&& points)
{
  if (points.size() == 0)
    points.push_back(Point(-1, -1));

  for (size_t i = 0; i < points.size(); i++)
  {
    for (size_t i = K - 1; i > 0; i--)
    {
      lastKAddedPoints[i] = std::move(lastKAddedPoints[i - 1]);
    }
    if (K > 0)
      lastKAddedPoints[0] = std::move(points[i]);
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


int Contour::findRight(const Point& value, int start, int count)
{
  int result = -1;

  count += start;
  if (points.size() < count)
    count = points.size();

  for (int i = start; i < count; i++)
  {
    if (value == points[points.size() - i])
    {
      result = points.size() - i;
      break;
    }
  }

  return result;
}