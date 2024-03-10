#pragma once
#include "ImageDataManager.h"
#include "Contour.h"
#include "Point.h"

class ContourDefiner
{

  ImageDataManager& imageManager;

public:

  ContourDefiner();
  ContourDefiner(ImageDataManager& imageManager);
  ~ContourDefiner();

  Contour defineContour(const Point& basePoint);
  
  Point getPointNearContour(const Point& startPoint);

  std::vector<Point> defineContourPointsAround(const Point& basePoint);

  std::vector<Point> convertToPath(const std::vector<Point> points);

  Point getNextPointInsideContour(const Point& basePoint, const std::vector<Point>& addedToContourPoints);
  
  Point getNextPointAfterChain(const Point& basePoint, const std::vector<Point>& pointChain, const Point& lastContourPoint);

  std::vector<Point> definePossiblePoints(const Point& basePoint);

  Point getFirstPointInChain(const Point& basePoint, const std::vector<Point>& pointChain);

  bool isInternalPoint(const Point& innerPoint, const Point& checkedPoint);

  template<class T>
  void removeIndexesFromVector(std::vector<T>& vector, std::vector<size_t>& indexes);

  Point getNextPoint(const Point& basePoint, const Point& predPoint);
};

