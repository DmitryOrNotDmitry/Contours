#pragma once
#include <vector>
#include <map>
#include "Contour.h"
#include "ContourState.h"

class DataStorageManager
{

  std::vector<Contour> contours;
  std::vector<Point> controlPoints;

  DataStorageManager();
  static DataStorageManager instance;

public:
  
  static DataStorageManager& getInstance();


  void addContour(Contour& contour);

  int getCountContours();

  std::vector<Contour>& getContours();



  void addControlPoint(const Point& point);

  std::vector<Point>& getControlPoints();

};

