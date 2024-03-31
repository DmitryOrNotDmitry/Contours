#pragma once
#include <vector>
#include "Point.h"
#include "ContourState.h"

class Contour
{

  std::vector<Point> points;

  ContourState state;

public:
  Contour();
  ~Contour();

  void addPoint(int x, int y);
  void addPoint(Point point);
  std::vector<Point> addPoints(std::vector<Point>& newPoints);

  Point getLastPoint();

  ContourState getState() const;
  void setState(ContourState state);

  size_t size() const;
  Point* getData();
  bool operator==(const Contour& other) const;
  bool operator!=(const Contour& other) const;
};

