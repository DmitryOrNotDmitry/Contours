#pragma once
#include <vector>
#include "LineBorder.h"

class LineBorderVector : public LineBorder
{

  std::vector<Point> points;

public:

  LineBorderVector(const std::vector<Point>& points);

  int getNextIdx(int curIndex, int step) const override;
  Point getPoint(int index) const override;

  int size() const override;
};

