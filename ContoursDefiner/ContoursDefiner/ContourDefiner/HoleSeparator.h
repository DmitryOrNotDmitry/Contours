#pragma once
#include <vector>
#include "Contour.h"

class HoleSeparator
{
public:

  static std::vector<Contour> separateToAtomicParts(const Contour& hole);

};

