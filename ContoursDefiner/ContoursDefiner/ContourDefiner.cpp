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

ContourDefiner::ContourDefiner(AbstractImageData* imageManager)
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
  
  Point lastAddedPoint = addContourPointsAround(startContourPoint, contour);
  Point currentInsidePoint = getNextPoint(startContourPoint, lastAddedPoint);

  int countIterations = 0;

  while (currentInsidePoint != startContourPoint)
  {
    lastAddedPoint = addContourPointsAround(currentInsidePoint, contour);
    if (lastAddedPoint == Point(-1, -1))
      lastAddedPoint = contour[contour.size() - 1];

    currentInsidePoint = getNextPoint(currentInsidePoint, lastAddedPoint);

    countIterations++;
    if (countIterations > MAX_ITERATIONS)
      break;
  }

  addContourPointsAround(currentInsidePoint, contour);

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


Point ContourDefiner::addContourPointsAround(const Point& basePoint, Contour& contour)
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

  if (contour.size() == 0)
    contour.addPoint(pointsForCheck[0]);

  int startIdx = 0;
  Point lastContourPoint = contour[contour.size() - 1];
  for (int i = 0; i < numPoints; i++)
  {
    if (lastContourPoint == pointsForCheck[i])
    {
      startIdx = (i + 1) % numPoints;
      break;
    }
  }
  
  int idx = startIdx;
  Point lastAddedPoint(-1, -1);
  for (int i = 0; i < numPoints; i++)
  {
    Point& checkedPoint = pointsForCheck[idx];
      
    if (contour[contour.size() - 1].DistanceTo(checkedPoint) < 1.999)
    {
      if (!isInternalPoint(basePoint, checkedPoint))
      {
        lastAddedPoint = checkedPoint;
        contour.addPoint(checkedPoint);
      }
    }
    else
    {
      break;
    }

    idx = (idx + 1) % numPoints;
  }

  return lastAddedPoint;
}


bool ContourDefiner::isInternalPoint(const Point& innerPoint, const Point& checkedPoint)
{
  return imageManager->getPointValue(innerPoint) == imageManager->getPointValue(checkedPoint);
}

Point ContourDefiner::getNextPoint(const Point& basePoint, const Point& lastContourPoint)
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

  int startIdx = 0;
  for (int i = 0; i < numPoints; i++)
  {
    if (lastContourPoint == pointsForCheck[i])
    {
      startIdx = (i + 1) % numPoints;
      break;
    }
  }

  int idx = startIdx;
  for (int i = 0; i < numPoints; i++)
  {
    if (isInternalPoint(basePoint, pointsForCheck[idx]))
      break;

    idx = (idx + 1) % numPoints;
  }

  return pointsForCheck[idx];
}
