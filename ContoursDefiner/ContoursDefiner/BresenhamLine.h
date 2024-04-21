#pragma once
#include "Point.h"

#include <vector>
#include <cmath>

class BresenhamLine
{

private:
  BresenhamLine() {};

public:

  static std::vector<Point> build(const Point& pointFrom, const Point& pointTo);

};

