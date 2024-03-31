#pragma once
#include "Contour.h"
#include "Point.h"

class ControlPointsCalculator
{

public:


  static std::pair<Point, Point> calculate(const int& firstIndex, const int& secondIndex);

};

