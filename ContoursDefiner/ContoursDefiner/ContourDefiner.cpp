#include "StdAfx.h"

#include "ContourDefiner.h"

#include <vector>
#include <algorithm>





ContourDefiner::ContourDefiner(HIMAGE hImage)
  : hImage(hImage)
{

}


ContourDefiner::~ContourDefiner()
{
}

void ContourDefiner::main()
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


  Point startPoint(192, 197);
  Contour contour;

  int x = startPoint.x;
  int y = startPoint.y;
  int chanValue = imageLines[y][x * QChans];
  while (chanValue == 255 && x < kolPix)
  {
    x += 1;

    chanValue = imageLines[y][x * QChans];
  }
  x--;
  Point startContourPoint(x, y);
  
  int color = 155;
  std::vector<Point> contourPoints = defineContourPointsAround(startContourPoint);
  Point currentInsidePoint = getNextPointInsideContour(startContourPoint, contourPoints);
  image.WriteDataStream(&color, startContourPoint.y, FORMAT_8, startContourPoint.x, 1);

   while (currentInsidePoint != startContourPoint)
  {
    contourPoints = defineContourPointsAround(currentInsidePoint);
    currentInsidePoint = getNextPointInsideContour(currentInsidePoint, contourPoints);
    image.WriteDataStream(&color, currentInsidePoint.y, FORMAT_8, currentInsidePoint.x, 1);
  }
  
  RecalcImageViews(hImage);

  for (int i = 0; i < kolPix; i++)
  {
    delete[] imageLines[i];
  }
  delete[] imageLines;
  
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
      contourPoints.push_back(checkedPoint);
  }

  return contourPoints;
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

  const Point& lastContourPoint = *contourPoints.crbegin();
  bool isPointCanBeNext = false;
  if (contourPoints.size() == 0)
    isPointCanBeNext = true;

  for (size_t i = 0; i < numPoints + 1; i++)
  {
    if (std::find(contourPoints.begin(), contourPoints.end(), pointsForCheck[i % numPoints]) == contourPoints.end())
    {
      if (isPointCanBeNext)
      {
        nextPoint = pointsForCheck[i % numPoints];
        break;
      }
    }
    else
      isPointCanBeNext = true;
  }

  return nextPoint;
}


int ContourDefiner::getPointValue(const Point& point, const int& offset)
{
  return imageLines[point.y][point.x * QChans + offset];
}


