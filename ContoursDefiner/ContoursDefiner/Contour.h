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
  std::vector<Point>& getPoints();

  Point getLastPoint();

  ContourState getState() const;
  void setState(ContourState state);

  size_t size() const;
  Point* getData();
  bool operator==(const Contour& other) const;
  bool operator!=(const Contour& other) const;
  Point& operator [](int i);
  Point operator [](int i) const;

  bool isEmpty() const;

  int findNearestPointTo(const Point& destination, int from, int to, int step) const;
  int findNearestPointTo(const Point& destination, int step = 1) const;
};

