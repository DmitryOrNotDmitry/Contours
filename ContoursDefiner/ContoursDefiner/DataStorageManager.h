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
  std::vector<std::vector<Point>> averageBorders;

  std::vector<Contour> holes;

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



  void addAverageBorder(std::vector<Point>&& averageBorder);

  std::vector<std::vector<Point>>& getAverageBorders();



  void addHole(Contour&& holes);

  std::vector<Contour>& getHoles();
};

