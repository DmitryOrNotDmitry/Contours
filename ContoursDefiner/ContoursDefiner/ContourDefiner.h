#pragma once
#include "ERInterface.h"
#include "Contour.h"
#include "Point.h"

class ContourDefiner
{

  HIMAGE hImage;

  unsigned char **imageLines;
  DWORD kolLines;
  DWORD kolPix;
  DWORD QChans;

public:

  ContourDefiner(HIMAGE hImage);
  ~ContourDefiner();

  void main();
  
  std::vector<Point> defineContourPointsAround(const Point& basePoint);

  Point getNextPointInsideContour(const Point& basePoint, const std::vector<Point>& contourPoints);

  int getPointValue(const Point& point, const int& offset = 0);

  bool containPoint(const std::vector<Point>& vector, const Point& point);

};

