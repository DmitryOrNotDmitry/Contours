#pragma once
#include "Contour.h"
#include "Point.h"
#include "LineBorder.h"


class GeneralBorderCalculator
{
public:
  static std::pair<LineBorder, LineBorder> defineNearBorders(Contour& first, Contour& second, double limitDist);

  static std::vector<std::pair<LineBorder, LineBorder>> defineAllGeneralBorders(Contour& first, Contour& second, double limitDist);
};

