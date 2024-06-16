#pragma once
#include "Contour.h"
#include "Point.h"
#include "LineBorder.h"

class GeneralBorderCalculator
{

  static bool haveContoursSameDirection(const Contour& first, const Contour& second);

  static std::pair<int, int> calculateNearestPointsIdx(const Contour& first, const Contour& second);

public:


  static std::pair<LineBorder, LineBorder> defineNearBorders(Contour& first, Contour& second, double limitDist);

  static std::vector<Point> averageTwoLine(LineBorder first, LineBorder second);

  

};

