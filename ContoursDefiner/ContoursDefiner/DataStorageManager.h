#pragma once
#include <vector>
#include <map>

//#include "Border.h"
#include "Contour.h"
#include "ContourState.h"


class DataStorageManager
{

  std::vector<Contour> contours;
  std::vector<Point> controlPoints;
  //std::vector<Border> borders;

  DataStorageManager();
  static DataStorageManager instance;

public:
  
  static DataStorageManager& getInstance();


  void addContour(Contour& contour);

  int getCountContours();

  std::vector<Contour>& getContours();



  void addControlPoint(const Point& point);

  std::vector<Point>& getControlPoints();



  /*void addBorder(const Border& border);

  std::vector<Border>& getBorders();*/
};

