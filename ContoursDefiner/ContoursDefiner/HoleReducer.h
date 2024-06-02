#pragma once
#include "Contour.h"

class HoleReducer
{
public:

  static void process(Contour& hole, std::list<Contour>& contours);
  static void processMulti(Contour& hole, std::list<Contour>& contours);

};

