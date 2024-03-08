#pragma once
#include <vector>
#include "Point.h"

class Contour
{

  std::vector<Point> points;

public:
  Contour();
  ~Contour();

  void addPoint(int x, int y);
  void addPoint(Point point);
  void addPoints(std::vector<Point>& newPoints);

  size_t size() const;
  Point* getData();
  bool operator==(const Contour& other) const;
  bool operator!=(const Contour& other) const;
};

