#pragma once
#include <vector>
#include <list>
#include <map>

#include "ContourDefiner/Contour.h"
#include "ContourState.h"
#include "ContourDefiner/LineBorder.h"
#include "ColorsPool.h"

struct ContourTraits
{
  ContourState state;
  int colorId;

  ContourTraits(ContourState state, int colorId) : state(state), colorId(colorId) {}
  ContourTraits() : state(ContourState::VISIBLE), colorId(0) {}
};

class DataStorageManager
{

  ColorsPool* colorsPool;

  std::list<Contour> contours;
  std::map<Contour*, ContourTraits> contoursStates;

  std::vector<LineBorder> borders;

  bool showHoles;
  std::vector<Contour> holes;

  DataStorageManager();
  static DataStorageManager instance;

  std::map<Contour*, Contour*> holeOwner;

public:
  
  static DataStorageManager& getInstance();


  void addContour(Contour& contour);

  int getCountContours();

  std::list<Contour>& getContours();
  std::vector<Contour*> getPContours();

  void setContourState(Contour& c, ContourState state);
  ContourState getContourState(Contour& c);

  COLORREF getContourColor(Contour& c);

  void clearState(Contour& c);

  void addBorder(const LineBorder& border);

  std::vector<LineBorder>& getBorders();


  bool isShowHoles();
  bool setShowHoles(bool showHoles);

  void addHole(Contour&& holes);
  void addHole(const Contour& holes);

  std::vector<Contour>& getHoles();

  COLORREF getHoleOwnerColor(Contour& hole);
  void setHoleOwner(Contour& hole, Contour& owner);
};

