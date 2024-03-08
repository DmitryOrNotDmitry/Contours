#include "StdAfx.h"

#include "ContourDefiner.h"

#include <vector>
#include <algorithm>
#include <map>

#define MAX_ITERATIONS 1000

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

  contour.addPoints(contourPoints);

  int countIterations = 0;

  //for(int i = 0; i < 396; i++)
  while (currentInsidePoint != startContourPoint)
  {
    contourPoints = convertToPath(defineContourPointsAround(currentInsidePoint));

    currentInsidePoint = getNextPointInsideContour(currentInsidePoint, contourPoints);

    contour.addPoints(contourPoints);

    countIterations++;
    if (countIterations > MAX_ITERATIONS)
      break;
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

  return nearContourPoint.toLeft();
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
    if (points.size() == 3)
      int tt = 0;

    std::sort(points_copy.begin(), points_copy.end());

    path.push_back(points_copy[0]);
    points_copy.erase(points_copy.begin());

    while (points_copy.size() > 0)
    {
      double minDistanceToLast = 10000 * 10000;
      double minDistanceToFirst = 10000 * 10000;
      size_t minItemIndexToLast = 0;
      size_t minItemIndexToFirst = 0;
      for (int i = 0; i < points_copy.size(); ++i)
      {
        double curDistToLast = points_copy[i].DistanceTo(*path.rbegin());
        double curDistToFirst = points_copy[i].DistanceTo(*path.begin());
        if (curDistToFirst < minDistanceToFirst)
        {
          minDistanceToFirst = curDistToFirst;
          minItemIndexToFirst = i;
        }
        if (curDistToLast < minDistanceToLast)
        {
          minDistanceToLast = curDistToLast;
          minItemIndexToLast = i;
        }
      }

      std::vector<size_t> indexesToDel;
      
      if (minItemIndexToLast == minItemIndexToFirst)
      {
        if (minDistanceToLast <= minDistanceToFirst)
        {
          path.insert(path.end(), points_copy[minItemIndexToLast]);
          indexesToDel.push_back(minItemIndexToLast);
        }
        else
        {
          path.insert(path.begin(), points_copy[minItemIndexToFirst]);
          indexesToDel.push_back(minItemIndexToFirst);
        }
      }
      else
      {
        double distMinItemToFirstTowardEndPath = points_copy[minItemIndexToFirst].DistanceTo(*path.rbegin());
        double distMinItemToLastTowardBeginPath = points_copy[minItemIndexToLast].DistanceTo(*path.begin());

        if (minDistanceToLast < distMinItemToLastTowardBeginPath)
        {
          path.insert(path.end(), points_copy[minItemIndexToLast]);
          indexesToDel.push_back(minItemIndexToLast);
        }

        if (minDistanceToFirst < distMinItemToFirstTowardEndPath)
        {
          path.insert(path.begin(), points_copy[minItemIndexToFirst]);
          indexesToDel.push_back(minItemIndexToFirst);
        }
      }
      
      

      

      removeIndexesFromVector(points_copy, indexesToDel);

    }
  }

  return path;
}

template<class T>
void ContourDefiner::removeIndexesFromVector(std::vector<T>& vector, std::vector<size_t>& indexes)
{
  std::sort(indexes.begin(), indexes.end());
  auto last = std::unique(indexes.begin(), indexes.end());
  indexes.erase(last, indexes.end());
  for (auto iter = indexes.rbegin(); iter != indexes.rend(); ++iter)
  {
    vector.erase(vector.begin() + *iter);
  }
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