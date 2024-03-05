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


void Contour::addPoints(std::vector<Point>& newPoints)
{
  if (newPoints.size() == 0)
    return;

  if (points.size() > 0 && newPoints.size() > 1)
  {
    if ((*points.rbegin()).DistanceTo(*newPoints.begin()) > (*points.rbegin()).DistanceTo(*newPoints.rbegin()))
      std::reverse(newPoints.begin(), newPoints.end());
  }

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
    size_t t = fitPoints.size();
    size_t tt = newPoints.size();

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
}


size_t Contour::size() const
{
  return points.size();
}


Point* Contour::getData()
{
  return points.data();
}