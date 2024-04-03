#pragma once
#include "Contour.h"
#include "Point.h"

class ControlPointsCalculator
{

  static std::pair<Point, Point> calculate(const Contour& first, const Contour& second);

public:

  //static std::pair<std::pair<int, int>, std::pair<int, int>> defineGeneralBorders(const Contour& first, const Contour& second);

};

