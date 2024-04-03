#pragma once
#include <vector>
#include <map>

#include "Contour.h"
#include "ContourState.h"
#include "LineBorder.h"

class DataStorageManager
{

  std::vector<Contour> contours;
  std::vector<Point> controlPoints;
  std::vector<LineBorder> borders;

  DataStorageManager();
  static DataStorageManager instance;

public:
  
  static DataStorageManager& getInstance();


  void addContour(Contour& contour);

  int getCountContours();

  std::vector<Contour>& getContours();



  void addControlPoint(const Point& point);

  std::vector<Point>& getControlPoints();



  void addBorder(const LineBorder& border);

  std::vector<LineBorder>& getBorders();
};

