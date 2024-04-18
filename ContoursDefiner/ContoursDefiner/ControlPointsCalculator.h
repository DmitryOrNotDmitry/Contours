#pragma once
#include "Contour.h"
#include "Point.h"

class GeneralBorderCalculator
{
  static std::pair<int, int> calculateNearestPointsIdx(const Contour& first, const Contour& second);

  static bool haveContoursSameDirection(const Contour& first, const Contour& second, const std::pair<int, int>& controlPointsIndexes);

public:

  static std::pair<std::pair<int, int>, std::pair<int, int>> defineGeneralBorders(const Contour& first, const Contour& second);

};

