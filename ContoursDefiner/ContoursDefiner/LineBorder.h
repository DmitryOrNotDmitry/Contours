#pragma once
#include "Contour.h"

class LineBorder
{

  std::vector<Contour>::iterator owner;
  int fromIndex;
  int toIndex;

public:

  LineBorder(std::vector<Contour>::iterator& owner, int fromIndex, int toIndex);

  std::vector<Contour>::iterator& getOwner();
  int getFromIndex();
  int getToIndex();

};

