#pragma once
#include "Contour.h"

class Border
{

  Contour& owner;
  int fromIndex;
  int toIndex;

public:

  Border(Contour& owner);
  Border(Contour& owner, int fromIndex, int toIndex);

  Contour& getOwner();
  int getFromIndex();
  int getToIndex();

};

