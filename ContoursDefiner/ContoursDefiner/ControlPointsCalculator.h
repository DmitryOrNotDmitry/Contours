#pragma once
#include "Contour.h"
#include "Point.h"

class ControlPointsCalculator
{

public:

  static std::pair<Point, Point> calculate(const Contour& first, const Contour& second);

};

