#pragma once
#include "GPC/gpc.h"
#include "Contour.h"

#include <vector>
#include <list>

class GPCAdapter
{
public:

  static std::vector<Contour> searchHoles(const std::list<Contour>& contours);

};

