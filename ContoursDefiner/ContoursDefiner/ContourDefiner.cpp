#include "StdAfx.h"

#include "ContourDefiner.h"

#include <vector>
#include <algorithm>
#include <map>





ContourDefiner::ContourDefiner(HIMAGE hImage)
  : hImage(hImage)
{
  CImageInterface image(hImage);

  kolLines = *image.dwImageKolLines;
  kolPix = *image.dwImageKolPix;
  QChans = *image.dwQChans;

  imageLines = new unsigned char*[kolLines];
  for (int i = 0; i < kolLines; i++)
  {
    imageLines[i] = new unsigned char[kolPix * QChans];
    image.ReadDataStream(imageLines[i], i, FORMAT_8);
  }
}


ContourDefiner::~ContourDefiner()
{
  for (int i = 0; i < kolPix; i++)
  {
    delete[] imageLines[i];
  }
  delete[] imageLines;
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

  int curChanValue = getPointValue(nearContourPoint);
  while (curChanValue == insideContuorColor && nearContourPoint.x < kolPix)
  {
    nearContourPoint = nearContourPoint.toRight();
    curChanValue = getPointValue(nearContourPoint);
  }
  nearContourPoint = nearContourPoint.toLeft();
  return nearContourPoint;
}


std::vector<Point> ContourDefiner::defineContourPointsAround(const Point& basePoint)
{
  std::vector<Point> contourPoints;
  contourPoints.reserve(8);

  int baseChanValue = getPointValue(basePoint);

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

    if (baseChanValue != getPointValue(checkedPoint))
    {
      contourPoints.push_back(checkedPoint);
    }
  }

  std::vector<Point> path;
  path.reserve(8);

  for (int i = 0; i < contourPoints.size(); ++i)
  {
    Point p = contourPoints[i];
    if (path.size() == 0)
    {
      path.push_back(contourPoints[i]);
    }
    else
    {
      double minDistance = kolLines * kolPix;
      size_t minIndex = 0;
      for (int j = 0; j < path.size(); ++j)
      {
        double currentDist = path[j].DistanceTo(contourPoints[i]);
        if (minDistance > currentDist)
        {
          minDistance = currentDist;
          minIndex = j;
        }
      }

      if (path.size() == 1)
      {
        if (contourPoints[i] < path[minIndex])
          path.insert(path.cbegin() + minIndex + 0, contourPoints[i]);
        else
          path.insert(path.cbegin() + minIndex + 1, contourPoints[i]);
      }
      else if (path[minIndex] == *path.begin())
      {
        if (minDistance < path[minIndex].DistanceTo(path[minIndex + 1]))
          path.insert(path.cbegin() + minIndex + 1, contourPoints[i]);
        else
          path.insert(path.cbegin() + minIndex + 0, contourPoints[i]);
      }
      else if (path[minIndex] == *path.rbegin())
      {
        if (minDistance < path[minIndex].DistanceTo(path[minIndex - 1]))
          path.insert(path.cbegin() + minIndex + 0, contourPoints[i]);
        else
          path.insert(path.cbegin() + minIndex + 1, contourPoints[i]);
      }
      else
      {
        double distToNext = path[minIndex].DistanceTo(path[minIndex + 1]);
        double distToPred = path[minIndex].DistanceTo(path[minIndex - 1]);
        if (distToNext < distToPred && minDistance < distToNext)
          path.insert(path.cbegin() + minIndex + 1, contourPoints[i]);
        else if (minDistance <= distToPred)
          path.insert(path.cbegin() + minIndex + 0, contourPoints[i]);
      }

    }
  }

  //for (int i = 0; i < contourPoints.size(); ++i)
  //  Point p = path[i];


  //Point firstInChain = getFirstPointInChain(basePoint, contourPoints);

  //if (firstInChain.x >= 0)
  //{
  //  size_t realContourPointsSize = contourPoints.size();
  //
  //  auto firstInChainIter = find(contourPoints.begin(), contourPoints.end(), firstInChain);
  //  contourPoints.insert(contourPoints.begin(), firstInChainIter, contourPoints.end());
  //
  //  contourPoints.resize(realContourPointsSize);
  //}

  
  //size_t j = 0;
  //for (; j < numPoints; j++)
  //{
  //  if (*contourPoints.begin() == pointsForCheck[j])
  //    break;
  //}

  //for (auto iter = contourPoints.begin(); iter != contourPoints.end(); iter++)
  //{
  //  if (*iter != pointsForCheck[j % numPoints])
  //  {
  //    contourPoints.resize(iter - contourPoints.begin());
  //    break;
  //  }

  //  j++;
  //}

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
    if (getPointValue(pointsForCheck[i]) == getPointValue(basePoint))
    {
      canPointBeTake[pointsForCheck[i + 0]] = true;
      
      Point predPoint;
      if (i == 0)
        predPoint = pointsForCheck[numPoints - 1];
      else
        predPoint = pointsForCheck[i - 1];

      if (getPointValue(predPoint) == getPointValue(basePoint))
        canPointBeTake[predPoint] = true;
      
      if (getPointValue(pointsForCheck[i + 1]) == getPointValue(basePoint))
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


int ContourDefiner::getPointValue(const Point& point, const int& offset)
{
  return imageLines[point.y][point.x * QChans + offset];
}


