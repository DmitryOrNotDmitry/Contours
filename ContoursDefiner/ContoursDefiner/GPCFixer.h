#pragma once
#include <vector>
#include <list>
#include "Contour.h"


struct Replacement
{
  Replacement(Point base, Point inserted) : base(base), inserted(inserted) { }

  Point base;
  Point inserted;
};



class GPCFixer
{
protected:

  std::vector<Replacement> replaces;

public:

  virtual void calcReplaces(std::vector<std::vector<char>>& matrix, Point XYmin) = 0;

  void modifyContours(const std::list<Contour>& contours, std::list<Contour>& newContours);

  void revertReplaces(std::vector<Contour>& holes);

  bool isEmpty() const;

  virtual bool skipContour(const Contour& contour, const Replacement& replace) const = 0;

};

