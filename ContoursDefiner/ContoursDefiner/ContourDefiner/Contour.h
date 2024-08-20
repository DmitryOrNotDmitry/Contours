#pragma once
#include "Point.h"

#include <vector>
#include <algorithm>

class Contour
{

  std::vector<Point> points;

  std::vector<Contour*> innerContours;

public:
  Contour();
  Contour(const Contour& other);
  ~Contour();

  Contour& operator=(const Contour& other);

  void addInnerContour(Contour* innerContour);
  Contour* getInnerContour(size_t idx);
  void removeInnerContour(size_t idx);

  void addPoint(Point point);
  void insertPoint(Point point, int index);

  std::vector<Point>& getPoints();

  Point getPoint(int index) const;
  int getNextIdx(int fromIndex, int step = 1) const;

  size_t size() const;
  
  bool operator==(const Contour& other) const;
  bool operator!=(const Contour& other) const;

  Point& operator [](int i);
  const Point& operator [](int i) const;

  bool isEmpty() const;

  int findNearestPointTo(const Point& destination, int from, int count, int step) const;
  int findNearestPointTo(const Point& destination, int step = 1) const;

  void removeSamePointsAtEnds();

  double distanceTo(const Point& destination) const;

  double area() const;
  double area(int from, int to) const;
  double signArea(int from, int to) const;

  bool isClockwise(int from, int to) const;
  bool isClockwise() const;

  std::vector<Contour*> calcNeighbors(std::vector<Contour>& contours);

  bool contains(const Point& point) const;
  bool isInner(const Point& point) const;

  void deletePins();

  int indexOf(const Point& point) const;
  int indexOf(const Point& point, int from, int count) const;

  int deletePoints(int from, int to);
  int distance(int from, int to) const;
  int minStep(int from, int to) const;

  std::vector<Contour> separate();

  void smooth(double epsilon, std::vector<Contour>& allContours);
};

