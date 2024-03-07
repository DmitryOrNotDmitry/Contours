#include "StdAfx.h"

#include "ContourDefiner.h"

#include <vector>
#include <algorithm>
#include <map>


ContourDefiner::ContourDefiner()
  : imageManager(ImageDataManager())
{
}

ContourDefiner::ContourDefiner(ImageDataManager& imageManager)
  : imageManager(imageManager)
{
}

ContourDefiner::~ContourDefiner()
{
}


Contour ContourDefiner::defineContour(const Point& startPoint)
{
  Contour contour;

  
  Point startContourPoint = getPointNearContour(startPoint);
  
  std::vector<Point> contourPoints = convertToPath(defineContourPointsAround(startContourPoint));  
  Point currentInsidePoint = getNextPointInsideContour(startContourPoint, contourPoints);
  
  if (contourPoints.size() > 1)
    contourPoints.erase(contourPoints.begin());

  contour.addPoints(contourPoints);

  //for(int i = 0; i < 396; i++)
  while (currentInsidePoint != startContourPoint)
  {
    contourPoints = convertToPath(defineContourPointsAround(currentInsidePoint));

    currentInsidePoint = getNextPointInsideContour(currentInsidePoint, contourPoints);

    contour.addPoints(contourPoints);
  }
  
  return contour;
}


Point ContourDefiner::getPointNearContour(const Point& startPoint)
{
  Point nearContourPoint = startPoint;

  while (isInternalPoint(startPoint, nearContourPoint) && nearContourPoint.x < imageManager.lineSize())
  {
    nearContourPoint = nearContourPoint.toRight();
  }

  return nearContourPoint.toLeft();;
}


std::vector<Point> ContourDefiner::defineContourPointsAround(const Point& basePoint)
{
  std::vector<Point> contourPoints;
  contourPoints.reserve(8);

  Point pointsForCheck[] = {
    basePoint.toRight(),
    basePoint.toRight().toBottom(),
    basePoint.toBottom(),
    basePoint.toBottom().toLeft(),
    basePoint.toLeft(),
    basePoint.toLeft().toUp(),
    basePoint.toUp(),
    basePoint.toUp().toRight(),
  };
  int numPoints = sizeof(pointsForCheck) / sizeof(pointsForCheck[0]);

  for (int i = 0; i < numPoints; i++)
  {
    Point& checkedPoint = pointsForCheck[i];

    if (!isInternalPoint(basePoint, checkedPoint))
    {
      contourPoints.push_back(checkedPoint);
    }
  }

  return contourPoints;
}


std::vector<Point> ContourDefiner::convertToPath(const std::vector<Point> points)
{
  std::vector<Point> path;
  path.reserve(8);

  
  if (points.size() > 0)
  {
    std::vector<Point> points_copy(points);
    path.push_back(points_copy[0]);
    points_copy.erase(points_copy.begin());

    while (points_copy.size() > 0)
    {
      double minDistance = 10000 * 10000;
      size_t minItemIndex = 0;
      for (int i = 0; i < points_copy.size(); ++i)
      {
        double currentDist = points_copy[i].DistanceTo(*path.rbegin());
        if (minDistance > currentDist)
        {
          minDistance = currentDist;
          minItemIndex = i;
        }
      }


      double distToFirst = points_copy[minItemIndex].DistanceTo(*path.begin());
      double distToLast = minDistance;

      if (distToLast > distToFirst)
      {
        path.insert(path.begin(), points_copy[minItemIndex]);
      }
      else
      {
        path.insert(path.end(), points_copy[minItemIndex]);
      }
      points_copy.erase(points_copy.begin() + minItemIndex);
    }
  }

  return path;
}


Point ContourDefiner::getNextPointInsideContour(const Point& basePoint, const std::vector<Point>& contourPoints)
{
  Point nextPoint;

  std::vector<Point> possibleNextPoints = definePossiblePoints(basePoint);

  nextPoint = getFirstPointInChain(basePoint, possibleNextPoints);

  return nextPoint;
}


std::vector<Point> ContourDefiner::definePossiblePoints(const Point& basePoint)
{
  std::vector<Point> possibleNextPoints;
  possibleNextPoints.reserve(8);
  
  Point pointsForCheck[] = {
    basePoint.toRight(),
    basePoint.toRight().toBottom(),
    basePoint.toBottom(),
    basePoint.toBottom().toLeft(),
    basePoint.toLeft(),
    basePoint.toLeft().toUp(),
    basePoint.toUp(),
    basePoint.toUp().toRight(),
  };
  int numPoints = sizeof(pointsForCheck) / sizeof(pointsForCheck[0]);
  
  std::map<Point, bool> canPointBeTake;
  for (int i = 0; i < numPoints; i++)
    canPointBeTake[pointsForCheck[i]] = false;
  
  for (int i = 0; i < numPoints; i += 2)
    if (isInternalPoint(basePoint, pointsForCheck[i]))
    {
      canPointBeTake[pointsForCheck[i + 0]] = true;
      
      Point predPoint;
      if (i == 0)
        predPoint = pointsForCheck[numPoints - 1];
      else
        predPoint = pointsForCheck[i - 1];

      if (isInternalPoint(basePoint, predPoint))
        canPointBeTake[predPoint] = true;
      
      if (isInternalPoint(basePoint, pointsForCheck[i + 1]))
        canPointBeTake[pointsForCheck[i + 1]] = true;
    }

  for (int i = 0; i < numPoints; i++)
    if (canPointBeTake[pointsForCheck[i]])
      possibleNextPoints.push_back(pointsForCheck[i]);
  
  return possibleNextPoints;
}


Point ContourDefiner::getFirstPointInChain(const Point& basePoint, const std::vector<Point>& pointChain)
{
  Point firstPoint;

  Point pointsForCheck[] = {
    basePoint.toRight(),
    basePoint.toRight().toBottom(),
    basePoint.toBottom(),
    basePoint.toBottom().toLeft(),
    basePoint.toLeft(),
    basePoint.toLeft().toUp(),
    basePoint.toUp(),
    basePoint.toUp().toRight(),
  };
  int numPoints = sizeof(pointsForCheck) / sizeof(pointsForCheck[0]);
  
  firstPoint = pointChain[0];

  int i = 0;
  while (pointsForCheck[i] != firstPoint)
    i++;

  for (size_t j = 0; j < pointChain.size(); j++)
  {
    if (pointChain[j] != pointsForCheck[i + j])
    {
      firstPoint = pointChain[j];
      break;
    }
  }

  return firstPoint;
}


bool ContourDefiner::isInternalPoint(const Point& innerPoint, const Point& checkedPoint)
{
  return imageManager.getPointValue(innerPoint) == imageManager.getPointValue(checkedPoint);
}