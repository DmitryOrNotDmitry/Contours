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
  /*std::vector<Point> path;
  path.reserve(8);

  if (points.size() == 0)
    return path;
    
  std::vector<Point> points_copy(points);
  path.push_back(points_copy[0]);
  points_copy.erase(points_copy.begin());

  while (points_copy.size() > 0)
  {
    Point& end_path = *path.rbegin();
    double min_dist = 10e20;
    size_t min_idx = 0;
    double cur_dist = 0;

    for (size_t i = 0; i < points_copy.size(); i++)
    {
      cur_dist = end_path.DistanceTo(points_copy[i]);
      if (cur_dist < min_dist)
      {
        min_dist = cur_dist;
        min_idx = i;
      }
    }

    path.push_back(points_copy[min_idx]);
    points_copy.erase(points_copy.begin() + min_idx);
  }


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


  std::reverse(path.begin(), path.end());
  return path;*/

  
  std::vector<Point> path;
  path.reserve(8);

  
  if (points.size() > 0)
  {
    std::vector<Point> points_copy(points);
    
    std::sort(points_copy.begin(), points_copy.end());

    path.push_back(points_copy[0]);
    points_copy.erase(points_copy.begin());

    while (points_copy.size() > 0)
    {
      double minDistanceToLast = 1e20;
      double minDistanceToFirst = 1e20;
      size_t minItemIndexToLast = 0;
      size_t minItemIndexToFirst = 0;
      for (size_t i = 0; i < points_copy.size(); ++i)
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

    /*Point& lastPoint = *path.rbegin();
    double distToNext = 0;
    double distToLast = 0;
    int count = 0;
    while (count < path.size())
    {
      count++;

      distToLast = path.begin()->DistanceTo(lastPoint);
      distToNext = path.begin()->DistanceTo(*(path.begin() + 1));
      if (distToLast <= distToNext)
      {
        Point tmp = *path.begin();
        path.erase(path.begin());
        path.push_back(tmp);
        lastPoint = *path.rbegin();
      }
      else
      {
        break;
      }
    }*/

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
