#include "LineBorder.h"

LineBorder::LineBorder(Contour& owner, int fromIndex, int toIndex)
  : owner(owner)
  , fromIndex(fromIndex)
  , toIndex(toIndex)
{
}

Contour& LineBorder::getOwner()
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
