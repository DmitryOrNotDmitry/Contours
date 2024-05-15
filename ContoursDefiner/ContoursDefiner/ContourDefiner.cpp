#include "StdAfx.h"

#include "ContourDefiner.h"

#include <vector>
#include <algorithm>
#include <map>

#define MAX_ITERATIONS 10000


ContourDefiner::ContourDefiner()
{
}

ContourDefiner::ContourDefiner(const ContourDefiner& other)
{
  this->imageManager = other.imageManager;
}

ContourDefiner::ContourDefiner(ImageDataManager* imageManager)
  : imageManager(imageManager)
{
}

ContourDefiner::~ContourDefiner()
{
}

ContourDefiner& ContourDefiner::operator=(const ContourDefiner& other)
{
  this->imageManager = other.imageManager;

  return *this;
}


Contour ContourDefiner::defineContour(const Point& startPoint)
{
  Contour contour;

  
  Point startContourPoint = getPointNearContour(startPoint);
  
  std::vector<Point> contourPoints = defineContourPointsAround(startContourPoint);

  contour.addPoints(convertToPath(contourPoints));

  Point tmp;
  Point currentInsidePoint = getNextPoint(startContourPoint, startContourPoint.toRight());
  Point predPoint = startContourPoint;

  int countIterations = 0;

  //for(int i = 0; i < 396; i++)
  while (currentInsidePoint != startContourPoint)
  {
    contourPoints = defineContourPointsAround(currentInsidePoint);

    contour.addPoints(convertToPath(contourPoints));
    
    tmp = currentInsidePoint;
    currentInsidePoint = getNextPoint(currentInsidePoint, predPoint);
    predPoint = tmp;

    countIterations++;
    if (countIterations > MAX_ITERATIONS)
      break;
  }

  contour.removeSamePointsAtEnds();
  
  return contour;
}


Point ContourDefiner::getPointNearContour(const Point& startPoint)
{
  Point nearContourPoint = startPoint;

  while (isInternalPoint(startPoint, nearContourPoint) && nearContourPoint.x < imageManager->lineSize())
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


std::vector<Point> ContourDefiner::convertToPath(const std::vector<Point>& points)
{
  std::vector<Point> path(points);

  std::vector<double> distances;
  size_t path_size = path.size();
  distances.resize(path_size);

  for (size_t i = 0; i < path_size; i++)
  {
    distances[i] = path[i].DistanceTo(path[(i + 1) % path_size]);
  }

  double max_dist = -1;
  double max_idx = 0;
  for (size_t i = 0; i < path_size; i++)
  {
    if (distances[i] > max_dist)
    {
      max_dist = distances[i];
      max_idx = i;
    }
  }

  for (size_t i = 0; i <= max_idx; i++)
  {
    Point tmp = *path.begin();
    path.erase(path.begin());
    path.push_back(tmp);
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


bool ContourDefiner::isInternalPoint(const Point& innerPoint, const Point& checkedPoint)
{
  return imageManager->getPointValue(innerPoint) == imageManager->getPointValue(checkedPoint);
}

Point ContourDefiner::getNextPoint(const Point& basePoint, const Point& predPoint)
{
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

  int i = 0;
  while (pointsForCheck[i % numPoints] != predPoint)
    i++;
  i += 1;

  for (int j = 0; j < numPoints; j++)
  {
    auto found_point = std::find(possibleNextPoints.begin(), possibleNextPoints.end(), pointsForCheck[(i + j) % numPoints]);
    if (found_point != possibleNextPoints.end())
    {
      return *found_point;
    }
  }
}
