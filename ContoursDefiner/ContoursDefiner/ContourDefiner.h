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

  std::vector<Point> defineContourPointsAround(const Point& basePoint);

  std::vector<Point> definePossiblePoints(const Point& basePoint);

  bool isInternalPoint(const Point& innerPoint, const Point& checkedPoint);

  template<class T>
  void removeIndexesFromVector(std::vector<T>& vector, std::vector<size_t>& indexes);

  Point getNextPoint(const Point& basePoint, const Point& predPoint);
};

