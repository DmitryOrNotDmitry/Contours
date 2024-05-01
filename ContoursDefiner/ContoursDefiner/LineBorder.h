#pragma once
#include "Contour.h"
#include "BresenhamLine.h"

#include <vector>

class LineBorder
{

  Contour& owner;
  int fromIndex;
  int toIndex;

public:

  LineBorder(Contour& owner, int fromIndex, int toIndex);

  LineBorder& operator=(const LineBorder& other);

  Contour& getOwner();
  int getFromIndex();
  int getToIndex();

  int getNextIdx(int curIndex, int step) const;
  Point getPoint(int index) const;

  void replaceLine(const std::vector<Point>& averagePoints);

  void reduceEnds(int countPoints);

  int size();

  static void reduceEndsWhileApproxTo(LineBorder& left, LineBorder& right);

private:

  void deleteContourLine();

};

