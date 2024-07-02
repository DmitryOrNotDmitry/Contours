#pragma once
#include "Point.h"


#include <vector>


class LineSmoother
{
  static double distanceToLine(const Point& point, const Point& startLine, const Point& endLine);

public:
  
  static std::vector<Point> DouglasPeucker(std::vector<Point>& points, double epsilon);

};



