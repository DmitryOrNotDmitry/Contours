#pragma once
#include "AbstractImageData.h"
#include "Contour.h"
#include "Point.h"

class ContourDefiner
{
protected:

  AbstractImageData* imageManager;

  Point getPointNearContour(const Point& startPoint);

  Point addContourPointsAround(const Point& basePoint, Contour& contour);

  Point getNextPoint(const Point& basePoint, const Point& lastContourPoint);

public:

  ContourDefiner();
  ContourDefiner(const ContourDefiner& other);
  ContourDefiner(AbstractImageData* imageManager);
  virtual ~ContourDefiner();

  ContourDefiner& operator=(const ContourDefiner& other);

  virtual Contour defineContour(const Point& startPoint);
};

