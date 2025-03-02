#pragma once

class Point
{
public:

  int x;
  int y;

  Point();
  Point(const Point& other);
  Point(int x, int y);
  
  ~Point();

  Point toRight(int offset = 1) const;
  Point toBottom(int offset = 1) const;
  Point toLeft(int offset = 1) const;
  Point toUp(int offset = 1) const;

  double DistanceTo(const Point& to) const;
  double squareDistanceTo(const Point& to) const;

  Point& operator=(const Point& other);
  Point& operator=(Point&& other);

  bool operator==(const Point& other) const;
  bool operator!=(const Point& other) const;
  bool operator<(const Point& other) const;

  Point operator+(const Point& other) const;
  Point& operator+=(const Point& other);
};

