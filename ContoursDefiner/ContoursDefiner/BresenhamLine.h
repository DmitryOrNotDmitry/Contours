#pragma once
#include "Point.h"
#include <vector>

class BresenhamLine
{

  std::vector<Point> line;

  void build(const Point& pointFrom, const Point& pointTo);

public:
  
  BresenhamLine(const Point& pointFrom, const Point& pointTo);

  Point operator[](int i) const;

  size_t size() const;

};

