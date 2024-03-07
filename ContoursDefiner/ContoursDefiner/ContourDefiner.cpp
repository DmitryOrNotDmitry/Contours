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
  
  std::vector<Point> contourPoints = defineContourPointsAround(startContourPoint);  
  Point currentInsidePoint = getNextPointInsideContour(startContourPoint, contourPoints);
  
  if (contourPoints.size() > 1)
    contourPoints.erase(contourPoints.begin());

  contour.addPoints(contourPoints);

  //for(int i = 0; i < 396; i++)
  while (currentInsidePoint != startContourPoint)
  {
    contourPoints = defineContourPointsAround(currentInsidePoint);

    currentInsidePoint = getNextPointInsideContour(currentInsidePoint, contourPoints);

    contour.addPoints(contourPoints);
  }
  
  return contour;
}


Point ContourDefiner::getPointNearContour(const Point& startPoint)
{
  Point nearContourPoint = startPoint;

  int insideContuorColor = 255;

  int curChanValue = imageManager.getPointValue(nearContourPoint);
  while (curChanValue == insideContuorColor && nearContourPoint.x < imageManager.lineSize())
  {
    nearContourPoint = nearContourPoint.toRight();
    curChanValue = imageManager.getPointValue(nearContourPoint);
  }
  nearContourPoint = nearContourPoint.toLeft();
  return nearContourPoint;
}


std::vector<Point> ContourDefiner::defineContourPointsAround(const Point& basePoint)
{
  std::vector<Point> contourPoints;
  contourPoints.reserve(8);

  int baseChanValue = imageManager.getPointValue(basePoint);

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

    if (baseChanValue != imageManager.getPointValue(checkedPoint))
    {
      contourPoints.push_back(checkedPoint);
    }
  }

  std::vector<Point> path = convertToPath(contourPoints);

  return path;
}


std::vector<Point> ContourDefiner::convertToPath(const std::vector<Point> points)
{
  std::vector<Point> path;
  path.reserve(8);

  for (int i = 0; i < points.size(); ++i)
  {
    Point p = points[i];
    if (path.size() == 0)
    {
      path.push_back(points[i]);
    }
    else
    {
      double minDistance = 10000 * 10000;
      size_t minIndex = 0;
      for (int j = 0; j < path.size(); ++j)
      {
        double currentDist = path[j].DistanceTo(points[i]);
        if (minDistance > currentDist)
        {
          minDistance = currentDist;
          minIndex = j;
        }
      }

      if (path.size() == 1)
      {
        if (points[i] < path[minIndex])
          path.insert(path.cbegin() + minIndex + 0, points[i]);
        else
          path.insert(path.cbegin() + minIndex + 1, points[i]);
      }
      else if (path[minIndex] == *path.begin())
      {
        if (minDistance < path[minIndex].DistanceTo(path[minIndex + 1]))
          path.insert(path.cbegin() + minIndex + 1, points[i]);
        else
          path.insert(path.cbegin() + minIndex + 0, points[i]);
      }
      else if (path[minIndex] == *path.rbegin())
      {
        if (minDistance < path[minIndex].DistanceTo(path[minIndex - 1]))
          path.insert(path.cbegin() + minIndex + 0, points[i]);
        else
          path.insert(path.cbegin() + minIndex + 1, points[i]);
      }
      else
      {
        double distToNext = path[minIndex].DistanceTo(path[minIndex + 1]);
        double distToPred = path[minIndex].DistanceTo(path[minIndex - 1]);
        double distToNextCurPoint = points[i].DistanceTo(path[minIndex + 1]);
        double distToPredCurPoint = points[i].DistanceTo(path[minIndex - 1]);
        if (distToNextCurPoint < distToNext || minDistance < distToNext)
          path.insert(path.cbegin() + minIndex + 1, points[i]);
        else if (distToPredCurPoint <= distToPred || minDistance < distToPred)
          path.insert(path.cbegin() + minIndex + 0, points[i]);
      }

    }
  }

  return path;
}


Point ContourDefiner::getNextPointInsideContour(const Point& basePoint, const std::vector<Point>& contourPoints)
{
  Point nextPoint;

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
    if (imageManager.getPointValue(pointsForCheck[i]) == imageManager.getPointValue(basePoint))
    {
      canPointBeTake[pointsForCheck[i + 0]] = true;
      
      Point predPoint;
      if (i == 0)
        predPoint = pointsForCheck[numPoints - 1];
      else
        predPoint = pointsForCheck[i - 1];

      if (imageManager.getPointValue(predPoint) == imageManager.getPointValue(basePoint))
        canPointBeTake[predPoint] = true;
      
      if (imageManager.getPointValue(pointsForCheck[i + 1]) == imageManager.getPointValue(basePoint))
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


