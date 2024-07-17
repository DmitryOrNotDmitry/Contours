#include "GeneralBorderCalculator.h"
#include <vector>
#include <algorithm>
#include <unordered_map>


struct Association
{
  Association(int idx1, int idx2, double dist)
    : idx1(idx1)
    , idx2(idx2)
    , dist(dist)
  {
  }

  bool operator==(const Association& other) const
  {
    return (idx1 == other.idx1) && (idx2 == other.idx2);
  }

  int idx1;
  int idx2;
  double dist;
};


std::pair<LineBorder, LineBorder> GeneralBorderCalculator::defineNearBorders(Contour& first, Contour& second, double limitDist)
{
  std::pair<LineBorder, LineBorder> result( LineBorder(first, 0, 0), LineBorder(second, 0, 0) );
  auto allBorders = defineAllGeneralBorders(first, second, 0);

  int maxBorderSize = -1;

  for (size_t i = 0; i < allBorders.size(); i++)
  {
    int curBorderSize = allBorders[i].first.size();
    if (curBorderSize > maxBorderSize)
    {
      result = allBorders[i];
      maxBorderSize = curBorderSize;
    }
  }

  return result;
}


std::vector<std::pair<LineBorder, LineBorder>> GeneralBorderCalculator::defineAllGeneralBorders(Contour& first, Contour& second, double limitDist)
{
  std::vector<std::pair<LineBorder, LineBorder>> result;
  std::vector<Association> assocs;

  if (first.size() == 0)
    return result;

  int len = first.size();
  int idx1 = 0;
  int idx2 = -1;
  double dist = 0;
  for (int i = 0; i < len; i++)
  {
    idx2 = second.findNearestPointTo(first.getPoint(idx1));
    dist = first.getPoint(idx1).DistanceTo(second.getPoint(idx2));

    if (dist < (limitDist + 0.001))
      assocs.push_back(Association(idx1, idx2, dist));

    idx1 = first.getNextIdx(idx1);
  }


  if (assocs.size() > 1)
  {
    int stepFromFirstToLast = second.minStep(assocs[0].idx2, assocs.rbegin()->idx2);
    if (std::abs(stepFromFirstToLast) <= 1)
    {
      size_t i = 0;
      for (; i < assocs.size() - 1; i++)
      {
        int stepToIdx2 = std::abs(second.minStep(assocs[i].idx2, assocs[i + 1].idx2));
        if (stepToIdx2 > 1)
          break;
      }

      if (i < (assocs.size() - 1))
      {
        std::vector<Association> replacedAssocs;
        replacedAssocs.insert(replacedAssocs.begin(), assocs.begin(), assocs.begin() + i + 1);

        assocs.erase(assocs.begin(), assocs.begin() + i + 1);

        assocs.insert(assocs.end(), replacedAssocs.begin(), replacedAssocs.end());
      }
    }
  }


  assocs.push_back(Association(-1000000, -1000000, 10e100));

  int curIdx1 = -1;
  int curIdx2 = -1;

  int predIdx1 = assocs[0].idx1;
  int predIdx2 = assocs[0].idx2;

  int startIdx = -1;
  int endIdx = -1;
  for (size_t i = 0; i < assocs.size(); i++)
  {
    curIdx1 = assocs[i].idx1;
    curIdx2 = assocs[i].idx2;

    int stepToIdx1 = std::abs(first.minStep(predIdx1, curIdx1));
    int stepToIdx2 = std::abs(second.minStep(predIdx2, curIdx2));

    if ((stepToIdx1 <= 1) && (stepToIdx2 <= 1))
    {
      if (startIdx == -1)
      {
        startIdx = i;
      }

      endIdx = i;
    }
    else
    {
      if (startIdx != -1)
      {
        int start = assocs[startIdx].idx1;
        int end = assocs[endIdx].idx1;

        LineBorder b1(first, start, end);

        if (b1.size() > 1)
        {
          int step = first.minStep(start, assocs[startIdx + 1].idx1);
          if (step < 0)
            b1 = LineBorder(first, end, start);
        }


        start = assocs[startIdx].idx2;
        end = assocs[endIdx].idx2;

        LineBorder b2_1(second, start, end);
        LineBorder b2_2(second, end, start);

        if (b1.size() > 2)
        {
          int step = first.distance(b1.getFromIndex(), b1.getToIndex()) / 2;
          int midIdx1 = first.getNextIdx(b1.getFromIndex(), step);
          int midIdx2 = second.findNearestPointTo(first.getPoint(midIdx1));

          if (b2_2.isInsideBorder(midIdx2) && !b2_1.isInsideBorder(midIdx2))
            b2_1 = b2_2;
        }
        else
        {
          int size2_1 = b2_1.size();
          int size2_2 = b2_2.size();

          if (std::abs(size2_2 - b1.size()) < std::abs(size2_1 - b1.size()))
            b2_1 = b2_2;
        }

        result.push_back(std::make_pair(b1, b2_1));
      }

      if (assocs[i].dist < (limitDist + 0.001))
      {
        startIdx = i;
        endIdx = i;
      }
      else
      {
        startIdx = -1;
        endIdx = -1;
      }
    }

    predIdx1 = curIdx1;
    predIdx2 = curIdx2;
  }

  return result;
}

