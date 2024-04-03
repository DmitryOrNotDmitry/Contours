#include "Border.h"

Border::Border(Contour& owner)
  : owner(owner) 
{
}

Border::Border(Contour& owner, int fromIndex, int toIndex)
  : owner(owner)
  , fromIndex(fromIndex)
  , toIndex(toIndex)
{
}

Contour& Border::getOwner()
{
  return owner;
}

int Border::getFromIndex()
{
  return fromIndex;
}

int Border::getToIndex()
{
  return toIndex;
}
