#pragma once
#include "BugContourDefiner.h"

class ImprovedBugContourDefiner : public BugContourDefiner
{
public:

  ImprovedBugContourDefiner(AbstractImageData* imageManager);
  ~ImprovedBugContourDefiner();

  Contour defineContour(const Point& startPoint) override;
};

