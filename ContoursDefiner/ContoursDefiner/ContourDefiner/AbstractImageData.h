#pragma once
#include "Point.h"

class AbstractImageData
{

public:

  virtual bool isContourPoint(const Point& point);

  virtual int lineSize();
  virtual int getCountLines();

  virtual ~AbstractImageData();

};

