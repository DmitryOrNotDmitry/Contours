#pragma once
#include "ImageDataManager.h"
#include "Contour.h"
#include "Point.h"

class ContourDefiner
{

  ImageDataManager* imageManager;

public:

  ContourDefiner();
  ContourDefiner(const ContourDefiner& other);
  ContourDefiner(ImageDataManager* imageManager);
  ~ContourDefiner();

  ContourDefiner& operator=(const ContourDefiner& other);

  Contour defineContour(const Point& basePoint);
  
  Point getPointNearContour(const Point& startPoint);

  Point addContourPointsAround(const Point& basePoint, Contour& contour);

  bool isInternalPoint(const Point& innerPoint, const Point& checkedPoint);

  Point getNextPoint(const Point& basePoint, const Point& lastContourPoint);
};

