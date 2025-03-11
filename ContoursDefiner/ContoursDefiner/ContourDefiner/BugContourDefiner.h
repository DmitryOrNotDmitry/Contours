#pragma once
#include "ContourDefiner.h"

class BugContourDefiner : public ContourDefiner
{
public:
  BugContourDefiner(AbstractImageData* imageManager);
  virtual ~BugContourDefiner();

  Contour defineContour(const Point& startPoint) override;

  void VectorCW(Point& point); // сдвиг вектора ПО часовой стрелке, каждая координата может иметь значения ТОЛЬКО -1, 0 и 1
  void VectorCCW(Point& point); // сдвиг вектора ПРОТИВ часовой стрелке, каждая координата может иметь значения ТОЛЬКО -1, 0 и 1
};

