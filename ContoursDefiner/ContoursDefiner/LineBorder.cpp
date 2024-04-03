#include "LineBorder.h"

LineBorder::LineBorder(std::vector<Contour>::iterator& owner, int fromIndex, int toIndex)
  : owner(owner)
  , fromIndex(fromIndex)
  , toIndex(toIndex)
{
}

std::vector<Contour>::iterator& LineBorder::getOwner()
{
  return owner;
}

int LineBorder::getFromIndex()
{
  return fromIndex;
}

int LineBorder::getToIndex()
{
  return toIndex;
}
