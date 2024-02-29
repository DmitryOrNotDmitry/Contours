#pragma once

class Point
{
public:

  int x;
  int y;

  Point();
  Point(int x, int y);
  //Point(const Point& other);
  
  ~Point();

  Point toRight(int offset = 1) const;
  Point toBottom(int offset = 1) const;
  Point toLeft(int offset = 1) const;
  Point toUp(int offset = 1) const;

  Point& operator=(const Point& other);
  
  bool operator==(const Point& other) const;
  bool operator!=(const Point& other) const;
};

