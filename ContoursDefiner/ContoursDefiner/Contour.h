#pragma once
#include "Point.h"
#include "ContourState.h"
#include "Rect.h"

#include <vector>
#include <list>
#include <algorithm>

class Contour
{

  std::vector<Point> points;
  
  ContourState state;

public:
  Contour();
  Contour(const Contour& other);
  ~Contour();

  Contour& operator=(const Contour& other);

  void addPoint(int x, int y);
  void addPoint(Point point);
  void insertPoint(Point point, int index);

  void setPoint(int idx, Point point);

  std::vector<Point>& getPoints();

  ContourState getState() const;
  void setState(ContourState state);

  size_t size() const;
  
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

  double area() const;
  double area(int from, int to) const;
  double signArea(int from, int to) const;

  bool isClockwise(int from, int to) const;
  bool isClockwise() const;

  void reverse();

  std::vector<Contour*> calcNeighbors(std::list<Contour>& contours);

  bool contains(const Point& point) const;
  bool isInner(const Point& point) const;

  void deletePins();

  int indexOf(const Point& point) const;
  int indexOf(const Point& point, int from, int count) const;

  int deletePoints(int from, int to);
  int distance(int from, int to) const;
  int minStep(int from, int to) const;

  std::vector<Contour> separate();

  std::pair<int, int> getBorderInsideRect(const Rect& rect);
  Rect defineRect() const;

  void smooth(double epsilon, std::list<Contour>& allContours);
};

