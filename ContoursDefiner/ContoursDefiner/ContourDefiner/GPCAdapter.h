#pragma once
#include "../GPC/gpc.h"
#include "Contour.h"

#include <vector>

class GPCAdapter
{
public:

  static std::vector<Contour> searchHoles(const std::vector<Contour*>& contours);

};

