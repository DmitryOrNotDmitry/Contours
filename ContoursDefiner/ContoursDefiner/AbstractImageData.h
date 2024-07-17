#pragma once
#include "Point.h"

class AbstractImageData
{

public:

  virtual int getPointValue(const Point& point, int offset = 0);

  virtual int lineSize();
  virtual int getCountLines();

  virtual ~AbstractImageData();

};

