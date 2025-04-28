#pragma once
#include "Point.h"
#include <vector>


class LineSmoother
{
  double distanceToLine(const Point& point, const Point& startLine, const Point& endLine) const;

  static LineSmoother* instance;

  LineSmoother();

public:

  static LineSmoother* Instance();
  
  std::vector<Point> DouglasPeucker(std::vector<Point>& points, double epsilon) const;

};



