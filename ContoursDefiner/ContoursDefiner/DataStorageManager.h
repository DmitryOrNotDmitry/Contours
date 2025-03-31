#pragma once
#include <vector>
#include <list>
#include <map>

#include "ContourDefiner/Contour.h"
#include "ContourState.h"
#include "ContourDefiner/LineBorder.h"

class DataStorageManager
{

  std::list<Contour> contours;
  std::map<Contour*, ContourState> contoursStates;

  std::vector<LineBorder> borders;

  bool showHoles;
  std::vector<Contour> holes;

  DataStorageManager();
  static DataStorageManager instance;

public:
  
  static DataStorageManager& getInstance();


  void addContour(Contour& contour);

  int getCountContours();

  std::list<Contour>& getContours();
  std::vector<Contour*> getPContours();

  void setContourState(Contour& c, ContourState state);
  
  ContourState getContourState(Contour& c);

  void clearState(Contour& c);

  void addBorder(const LineBorder& border);

  std::vector<LineBorder>& getBorders();


  bool isShowHoles();
  bool setShowHoles(bool showHoles);

  void addHole(Contour&& holes);
  void addHole(const Contour& holes);

  std::vector<Contour>& getHoles();
};

