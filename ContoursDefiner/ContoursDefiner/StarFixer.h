#pragma once
#include "GPCFixer.h"


class StarFixer : public GPCFixer
{

public:
  
  void calcReplaces(std::vector<std::vector<char>>& matrix, Point XYmin) override;

  bool skipContour(const Contour& contour, const Replacement& replace) const override;

};

