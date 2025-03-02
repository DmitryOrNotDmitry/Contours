#pragma once
#include "ContourDefiner/ContourDefiner.h"

class BugContoursDefiner : public ContourDefiner
{
public:

  BugContoursDefiner(AbstractImageData* imageManager);
  ~BugContoursDefiner();

  Contour defineContour(const Point& startPoint) override;
};

