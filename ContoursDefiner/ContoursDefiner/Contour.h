#pragma once
#include <vector>
#include <algorithm>
#include "Point.h"
#include "ContourState.h"

#define K 4

class Contour
{

  std::vector<Point> points;
  
  std::vector<Point> lastKAddedPoints;

  ContourState state;

public:
  Contour();
  ~Contour();

  void addPoint(int x, int y);
  void addPoint(Point point);
  void insertPoint(Point point, int index);
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

  void removeSamePointsAtEnds();

  bool haveRepeatPoints();

private:

  void memoryLastAddedPoints(std::vector<Point>&& points);

  void deleteYetAddedPoints(std::vector<Point>& deletedPoints);


  int findRight(const Point& value, int start = 0, int count=20);

};

