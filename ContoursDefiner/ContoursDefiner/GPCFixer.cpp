#include "GPCFixer.h"

void GPCFixer::modifyContours(const std::list<Contour>& contours, std::list<Contour>& newContours)
{
  double prevDist, nextDist;
  for (size_t i = 0; i < replaces.size(); i++)
  {
    Point insertedPoint = replaces[i].inserted;
    for (auto iter = newContours.begin(); iter != newContours.end(); ++iter)
    {
      int baseIdx = iter->indexOf(replaces[i].base);
      if (baseIdx == -1)
        continue;

      if (skipContour(*iter, replaces[i]))
        continue;

      prevDist = insertedPoint.DistanceTo(iter->getPoint(iter->getNextIdx(baseIdx, -1)));
      nextDist = insertedPoint.DistanceTo(iter->getPoint(iter->getNextIdx(baseIdx)));

      if (prevDist < nextDist)
      {
        iter->insertPoint(insertedPoint, baseIdx);
      }
      else
      {
        iter->insertPoint(insertedPoint, baseIdx + 1);
      }
    }
  }
}

void GPCFixer::revertReplaces(std::vector<Contour>& holes)
{
  for (size_t i = 0; i < replaces.size(); i++)
  {
    for (auto iter = holes.begin(); iter != holes.end(); ++iter)
    {
      int insertedIdx = iter->indexOf(replaces[i].inserted);
      if (insertedIdx != -1)
        iter->setPoint(insertedIdx, replaces[i].base);
    }
  }
}

bool GPCFixer::isEmpty() const
{
  return replaces.empty();
}
