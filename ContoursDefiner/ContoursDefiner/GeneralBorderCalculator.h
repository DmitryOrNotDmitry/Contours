#pragma once
#include "Contour.h"
#include "Point.h"
#include "LineBorder.h"


struct Association
{
  Association(int idx1, int idx2, double dist)
    : idx1(idx1)
    , idx2(idx2)
    , dist(dist)
  {
  }

  bool operator==(const Association& other) const
  {
    return (idx1 == other.idx1) && (idx2 == other.idx2);
  }

  int idx1;
  int idx2;
  double dist;
};


class GeneralBorderCalculator
{

  static bool haveContoursSameDirection(const Contour& first, const Contour& second);

  static std::pair<int, int> calculateNearestPointsIdx(const Contour& first, const Contour& second);

public:


  static std::pair<LineBorder, LineBorder> defineNearBorders(Contour& first, Contour& second, double limitDist);

  static std::vector<Point> averageTwoLine(LineBorder first, LineBorder second);

  static std::vector<std::pair<LineBorder, LineBorder>> defineGeneralBorders(Contour& first, Contour& second, double limitDist);

  static void sortAssociationsByIdx2(std::vector<Association>& assocs);

};

