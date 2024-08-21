#include "AbstractImageData.h"


bool AbstractImageData::isContourPoint(const Point& point)
{
  return false;
}

int AbstractImageData::lineSize()
{
  return 0;
}

int AbstractImageData::getCountLines()
{
  return 0;
}

AbstractImageData::~AbstractImageData()
{
}
