#pragma once
#include "ContourDefiner.h"

class BugContourDefiner : public ContourDefiner
{
public:
  BugContourDefiner(AbstractImageData* imageManager);
  virtual ~BugContourDefiner();

  Contour defineContour(const Point& startPoint) override;

  void VectorCW(Point& point); // ����� ������� �� ������� �������, ������ ���������� ����� ����� �������� ������ -1, 0 � 1
  void VectorCCW(Point& point); // ����� ������� ������ ������� �������, ������ ���������� ����� ����� �������� ������ -1, 0 � 1
};

