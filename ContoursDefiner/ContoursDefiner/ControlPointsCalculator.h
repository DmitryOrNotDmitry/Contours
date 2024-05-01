#pragma once
#include "Contour.h"
#include "Point.h"
#include "LineBorder.h"

class GeneralBorderCalculator
{
  static std::pair<int, int> calculateNearestPointsIdx(const Contour& first, const Contour& second);

  static bool haveContoursSameDirection(const Contour& first, const Contour& second, const std::pair<int, int>& controlPointsIndexes);

public:

  static std::pair<LineBorder, LineBorder> defineNearBorders(Contour& first, Contour& second);

  static std::vector<Point> averageTwoLine(LineBorder first, LineBorder second);

  

};

