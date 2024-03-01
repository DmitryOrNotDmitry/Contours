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


void Contour::addPoints(const std::vector<Point>& newPoints)
{
  std::vector<Point> fitPoints;
  size_t numNewPoints = newPoints.size();
  if (points.size() < numNewPoints)
    numNewPoints = points.size();

  for (auto iter = newPoints.begin(); iter != newPoints.end(); iter++)
  {
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
      fitPoints.push_back(*iter);
  }

  points.insert(points.cend(), fitPoints.begin(), fitPoints.end());
}


size_t Contour::size() const
{
  return points.size();
}


Point* Contour::getData()
{
  return points.data();
}