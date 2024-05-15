#pragma once
#include "Point.h"

#include <vector>

class Path
{

  std::vector<Point> points;

public:

  Path(const std::vector<Point>& pointsToPath);
  ~Path();

  size_t size() const
  {
    return points.size();
  }

  Point& operator[](int i)
  {
    if (i < 0)
      i += size();

    return points[i];
  }

  void reverse()
  {
    std::reverse(points.begin(), points.end());
  }

  std::vector<Point>::iterator begin()
  {
    return points.begin();
  }

  std::vector<Point>::iterator end()
  {
    return points.end();
  }

  bool operator==(const Path& other) const;
  bool operator==(const std::vector<Point>& vector) const;

};

