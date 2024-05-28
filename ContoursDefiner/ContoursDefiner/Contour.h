#pragma once
#include <vector>
#include <list>
#include <algorithm>
#include "Point.h"
#include "ContourState.h"

#define K 4

class Contour
{

  std::vector<Point> points;
  
  std::vector<Point> lastKAddedPoints;

  ContourState state;

  void memoryLastAddedPoints(std::vector<Point>&& points);

  void deleteYetAddedPoints(std::vector<Point>& deletedPoints);

public:
  Contour();
  ~Contour();

  void addPoint(int x, int y);
  void addPoint(Point point);
  void insertPoint(Point point, int index);

  std::vector<Point> addPoints(std::vector<Point>& newPoints);
  std::vector<Point>& getPoints();

  ContourState getState() const;
  void setState(ContourState state);

  size_t size() const;
  Point* getData();
  
  bool operator==(const Contour& other) const;
  bool operator!=(const Contour& other) const;

  Point& operator [](int i);
  const Point& operator [](int i) const;

  bool isEmpty() const;

  int findNearestPointTo(const Point& destination, int from, int count, int step) const;
  int findNearestPointTo(const Point& destination, int step = 1) const;

  void removeSamePointsAtEnds();

  bool haveRepeatPoints() const;

  Point getPoint(int index) const;
  int getNextIdx(int fromIndex, int step = 1) const;

  double distanceTo(const Point& destination) const;

  Point getAvaragePoint();

  double area();
  double area(int from, int to);

  std::vector<Contour*> calcNeighbors(std::list<Contour>& contours);

  bool contains(const Point& point) const;
  bool isInner(const Point& point) const;

  void deletePins();

  int indexOf(const Point& point, int from, int count) const;

  int deletePoints(int from, int to);
  int distance(int from, int to) const;

  std::vector<Contour> separate();
};

