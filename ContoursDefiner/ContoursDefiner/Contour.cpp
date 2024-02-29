#include "StdAfx.h"
#include "Contour.h"


Contour::Contour()
{
}


Contour::~Contour()
{
}


void Contour::addPoint(Point point)
{
  points.push_back(point);
}


void Contour::addPoint(int x, int y)
{
  points.push_back(Point(x, y));
}
