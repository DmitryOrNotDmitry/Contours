#pragma once
#include "Contour.h"

class LineBorder
{

  Contour& owner;
  int fromIndex;
  int toIndex;

public:

  LineBorder(Contour& owner, int fromIndex, int toIndex);

  Contour& getOwner();
  int getFromIndex();
  int getToIndex();

};

