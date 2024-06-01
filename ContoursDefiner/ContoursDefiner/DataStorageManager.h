#pragma once
#include <vector>
#include <list>

#include "Contour.h"
#include "ContourState.h"
#include "LineBorder.h"

class DataStorageManager
{

  std::list<Contour> contours;

  std::vector<LineBorder> borders;

  std::vector<Contour> holes;

  DataStorageManager();
  static DataStorageManager instance;

public:
  
  static DataStorageManager& getInstance();


  void addContour(Contour& contour);

  int getCountContours();

  std::list<Contour>& getContours();


  void addBorder(const LineBorder& border);

  std::vector<LineBorder>& getBorders();


  void addHole(Contour&& holes);
  void addHole(const Contour& holes);

  std::vector<Contour>& getHoles();
};

