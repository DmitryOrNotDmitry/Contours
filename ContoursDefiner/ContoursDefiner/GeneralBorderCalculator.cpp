#include "GeneralBorderCalculator.h"
#include <vector>
#include <algorithm>
#include <unordered_map>

std::pair<bool, bool> isNonDecreasing(LineBorder& first, LineBorder& second, const std::vector<Association>& assocs, double limitDist);

std::vector<std::pair<LineBorder, LineBorder>> algoritm1(Contour& first, Contour& second, double limitDist);
std::vector<std::pair<LineBorder, LineBorder>> algoritm2(Contour& first, Contour& second, double limitDist);


struct ContourBorderAttrs {

  ContourBorderAttrs(const Contour& contour, int index, int breakIndex, int step)
    : contour(contour)
    , index(index)
    , breakIndex(breakIndex)
    , step(step)
  {
  }

  const Contour& contour;
  int index;
  int breakIndex;
  int step;
};

void calcBordersEnds(ContourBorderAttrs& contAttr1, ContourBorderAttrs& contAttr2, double maxDist)
{
  int startIndex = contAttr1.breakIndex;
  double dist = 0;

  while (true)
  {
    int possibleIndex1 = contAttr1.contour.getNextIdx(contAttr1.index, contAttr1.step);
    int nearPointTo1 = contAttr2.contour.findNearestPointTo(contAttr1.contour[possibleIndex1]);

    dist = contAttr1.contour[possibleIndex1].DistanceTo(contAttr2.contour[nearPointTo1]);
    if (dist >= maxDist)
      break;

    if (possibleIndex1 == startIndex)
      break;

    contAttr1.index = possibleIndex1;
  }

  startIndex = contAttr2.breakIndex;
  dist = 0;

  while (true)
  {
    int possibleIndex2 = contAttr2.contour.getNextIdx(contAttr2.index, contAttr2.step);
    int nearPointTo2 = contAttr1.contour.findNearestPointTo(contAttr2.contour[possibleIndex2]);

    dist = contAttr2.contour[possibleIndex2].DistanceTo(contAttr1.contour[nearPointTo2]);
    if (dist >= maxDist)
      break;

    if (possibleIndex2 == startIndex)
      break;

    contAttr2.index = possibleIndex2;
  }
}


std::pair<int, int> GeneralBorderCalculator::calculateNearestPointsIdx(const Contour& first, const Contour& second)
{
  std::pair<int, int> result;
  result.first = -1;
  result.second = -1;

  if (first.isEmpty() || second.isEmpty())
    return result;
  
  int firstIndex = 0;
  int secondNearPointIdx = second.findNearestPointTo(first[firstIndex]);

  double dist = first[firstIndex].DistanceTo(second[secondNearPointIdx]);
  double minDist = dist;

  result.first = firstIndex;
  result.second = secondNearPointIdx;

  size_t countPassedPoints = 0;
  while (dist > 0 && countPassedPoints < first.size())
  {
    int step = static_cast<int>(floor(dist));
    firstIndex = first.getNextIdx(firstIndex, step);
    secondNearPointIdx = second.findNearestPointTo(first[firstIndex]);

    dist = first[firstIndex].DistanceTo(second[secondNearPointIdx]);
    if (dist < minDist)
    {
      minDist = dist;
      result.first = firstIndex;
      result.second = secondNearPointIdx;
    }

    countPassedPoints += step;
  }
  
  if (minDist > 0)
  {
    int count = (int)minDist * 2;
    if (count > first.size())
      count = first.size();
    result.first = first.findNearestPointTo(second[result.second], first.getNextIdx(result.first, -count / 2), count, 1);

    count = (int)minDist * 2;
    if (count > second.size())
      count = second.size();
    result.second = second.findNearestPointTo(first[result.first], second.getNextIdx(result.second, -count / 2), count, 1); 
  }
  
  return result;
}


bool GeneralBorderCalculator::haveContoursSameDirection(const Contour& first, const Contour& second)
{
  return first.isClockwise() == second.isClockwise();
}


std::pair<LineBorder, LineBorder> GeneralBorderCalculator::defineNearBorders(Contour& first, Contour& second, double limitDist)
{
  std::pair<LineBorder, LineBorder> result1( LineBorder(first, 0, 0), LineBorder(second, 0, 0) );
  auto allBorders = defineGeneralBorders(first, second, 0);

  int maxBorderSize = -1;

  for (size_t i = 0; i < allBorders.size(); i++)
  {
    int curBorderSize = allBorders[i].first.size();
    if (curBorderSize > maxBorderSize)
    {
      result1 = allBorders[i];
      maxBorderSize = curBorderSize;
    }
  }

  return result1;

  std::pair<LineBorder, LineBorder> result(LineBorder(first, 0, 0), LineBorder(second, 0, 0));
  std::pair<int, int> firstBorder;
  std::pair<int, int> secondBorder;


  std::pair<int, int> controlPoints = GeneralBorderCalculator::calculateNearestPointsIdx(first, second);

  int stepFirst = 1;
  int stepSecond = -1;
  //if (haveContoursSameDirection(first, second))
  //  stepSecond = 1;
  
  ContourBorderAttrs firstContAttrs(first, controlPoints.first, controlPoints.first, stepFirst);
  ContourBorderAttrs secondContAttrs(second, controlPoints.second, controlPoints.second, stepSecond);

  calcBordersEnds(firstContAttrs, secondContAttrs, limitDist);

  firstBorder.second = firstContAttrs.index;
  secondBorder.first = secondContAttrs.index;



  firstContAttrs.breakIndex = firstContAttrs.index;
  secondContAttrs.breakIndex = secondContAttrs.index;

  firstContAttrs.index = controlPoints.first;
  secondContAttrs.index = controlPoints.second;
  
  firstContAttrs.step = -stepFirst;
  secondContAttrs.step = -stepSecond;

  calcBordersEnds(firstContAttrs, secondContAttrs, limitDist);
  

  firstBorder.first = firstContAttrs.index;
  secondBorder.second = secondContAttrs.index;
  
  result.first = LineBorder(first, firstBorder.first, firstBorder.second);
  result.second = LineBorder(second, secondBorder.first, secondBorder.second);

  LineBorder::reduceEndsWhileApproxTo(result.first, result.second, limitDist * 2);
  
  if (first.size() == result.first.size())
  {
    result.first.agreeWith(result.second);
  }

  if (second.size() == result.second.size())
  {
    result.second.agreeWith(result.first);
  }
  
  return result;
}

std::vector<Point> GeneralBorderCalculator::averageTwoLine(LineBorder first, LineBorder second)
{
  std::vector<Point> result;
  result.reserve(first.size());

  int step = 1;
  int firstIndex = first.getFromIndex();
  int secondIndex = second.getToIndex();

  while (firstIndex != first.getToIndex())
  {
    Point& point1 = first.getPoint(firstIndex);
    Point& point2 = second.getPoint(secondIndex);
    
    result.push_back(Point( (point1.x + point2.x) / 2, (point1.y + point2.y) / 2 ));

    firstIndex = first.getNextIdx(firstIndex, step);
    secondIndex = second.getNextIdx(secondIndex, -step);
  }

  return result;
}


void addNewAssoc(const Contour& first, const Contour& second, std::vector<Association>& asscos, int idx2)
{
  int idx1 = first.findNearestPointTo(second.getPoint(idx2));
  double dist = first.getPoint(idx1).DistanceTo(second.getPoint(idx2));

  asscos.push_back(Association(idx1, idx2, dist));
}


std::vector<std::pair<LineBorder, LineBorder>> GeneralBorderCalculator::defineGeneralBorders(Contour& first, Contour& second, double limitDist)
{
  std::vector<std::pair<LineBorder, LineBorder>> result;

  std::vector<Association> assocs;

  Rect firstRect = first.defineRect();
  Rect secondRect = second.defineRect();
  Rect intersectRect = firstRect.intersect(secondRect);

  if (intersectRect == Rect::nullRect)
    return result;

  std::pair<int, int> indexes = first.getBorderInsideRect(intersectRect);
  LineBorder firstBorder(first, indexes.first, indexes.second);
  
  indexes = second.getBorderInsideRect(intersectRect);
  LineBorder secondBorder(second, indexes.first, indexes.second);

  return algoritm2(first, second, limitDist);
  //if (firstBorder.size() >= secondBorder.size())
  //{
  //  return qweqwe(first, second, limitDist);
  //}
  //else
  //{
  //  return qweqwe(second, first, limitDist);
  //}
}


std::vector<std::pair<LineBorder, LineBorder>> algoritm2(Contour& first, Contour& second, double limitDist)
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

std::vector<std::pair<LineBorder, LineBorder>> algoritm1(Contour& first, Contour& second, double limitDist)
{
  std::vector<std::pair<LineBorder, LineBorder>> result;
  std::vector<Association> assocs;
  
  Rect firstRect = first.defineRect();
  Rect secondRect = second.defineRect();
  Rect intersectRect = firstRect.intersect(secondRect);

  std::pair<int, int> indexes = first.getBorderInsideRect(intersectRect);
  LineBorder firstBorder(first, indexes.first, indexes.second);

  indexes = second.getBorderInsideRect(intersectRect);
  LineBorder secondBorder(second, indexes.first, indexes.second);

  int borderLen = firstBorder.size();
  int idx1 = firstBorder.getFromIndex();
  int idx2 = -1;
  double dist = 0;
  for (int i = 0; i < borderLen; i++)
  {
    idx2 = second.findNearestPointTo(first.getPoint(idx1));
    dist = first.getPoint(idx1).DistanceTo(second.getPoint(idx2));

    assocs.push_back(Association(idx1, idx2, dist));

    idx1 = firstBorder.getNextIdx(idx1);
  }

  auto isAsceding = isNonDecreasing(firstBorder, secondBorder, assocs, limitDist);

  bool isFirstAsceding = isAsceding.first;
  bool isSecondAsceding = isAsceding.second;
  //bool isFirstAsceding = isAsceding.first;
  //bool isSecondAsceding = isAsceding.second;

  std::vector<Association> newAssocs;

  int predIdx2 = assocs[0].idx2;
  int curIdx2 = -1;
  for (size_t i = 0; i < assocs.size(); i++)
  {
    curIdx2 = assocs[i].idx2;

    if ((assocs[i].dist - 0.001) > limitDist)
    {
      predIdx2 = curIdx2;
      continue;
    }

    int distance = std::abs(second.distance(predIdx2, curIdx2));
    int distance2 = std::abs(second.distance(curIdx2, predIdx2));
    if (distance2 < distance)
      distance = distance2;

    if (distance > 1)
    {
      int newIdx2 = -1;
      if (isSecondAsceding) //44, 47, 48, 42
      {
        if (predIdx2 > curIdx2)
        {
          int j = predIdx2 + 1;
          if (j < secondBorder.getToIndex() + 1)
          {
            addNewAssoc(first, second, newAssocs, j);
          }

          j = second.getNextIdx(curIdx2, -1);
          addNewAssoc(first, second, newAssocs, j);


        }
        else
        {
          int j = predIdx2 + 1;
          if (j < curIdx2)
          {
            addNewAssoc(first, second, newAssocs, j);
          }
        }

      }
      else //42, 48, 47, 44
      {

        if (predIdx2 < curIdx2)
        {
          int j = predIdx2 - 1;
          if (j > secondBorder.getFromIndex() - 1)
          {
            addNewAssoc(first, second, newAssocs, j);
          }

          j = second.getNextIdx(curIdx2, 1);
          addNewAssoc(first, second, newAssocs, j);
        }
        else
        {
          int j = predIdx2 - 1;
          if (j > curIdx2)
          {
            addNewAssoc(first, second, newAssocs, j);
          }
        }

      }

      //addNewAssoc(first, second, newAssocs, newIdx2);
    }

    //if (newAssocs.size() > 0)
    //{
    //  assocs.insert(assocs.begin() + i, newAssocs.begin(), newAssocs.end());

    //  i += newAssocs.size();
    //  newAssocs.clear();
    //}

    predIdx2 = curIdx2;
  }

  assocs.insert(assocs.end(), newAssocs.begin(), newAssocs.end());
  
  if (newAssocs.size() > 0)
    GeneralBorderCalculator::sortAssociationsByIdx2(assocs);


  if (assocs.size() > 1)
  {
    if (firstBorder.size() == first.size() || secondBorder.size() == second.size())
    {
      int i = assocs.size() - 1;
      for (; i >= 0; i--)
        if (assocs[i].dist > (limitDist + 0.001))
          break;

      if (i > 0 && i < (assocs.size() - 1))
      {
        std::vector<Association> replacedAssocs;
        replacedAssocs.insert(replacedAssocs.begin(), assocs.begin() + i + 1, assocs.end());
        assocs.erase(assocs.begin() + i + 1, assocs.end());
        assocs.insert(assocs.begin(), replacedAssocs.begin(), replacedAssocs.end());
      }
    }
  }

  assocs.push_back(Association(-1, -1, 10e100));

  double lastDist = -1;
  int startIdx = -1;
  int endIdx = -1;
  for (size_t i = 0; i < assocs.size(); i++)
  {
    Association& assoc = assocs[i];

    if (assoc.dist <= (limitDist + 0.001))
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

      startIdx = -1;
      endIdx = -1;
    }
  }


  return result;
}


std::pair<bool, bool> isNonDecreasing(LineBorder& first, LineBorder& second, const std::vector<Association>& assocs, double limitDist)
{
  std::pair<bool, bool> result = std::make_pair(false, false);
  
  int countIdx1 = 0;
  int countIdx2 = 0;
  
  for (size_t i = 0; i < assocs.size() - 1; i++)
  {
    countIdx1 += first.minStep(assocs[i].idx1, assocs[i + 1].idx1);

    countIdx2 += second.minStep(assocs[i].idx2, assocs[i + 1].idx2);
  }

  result.first = countIdx1 >= 0;
  result.second = countIdx2 >= 0;

  //int countWrongs = 0;
  //for (size_t i = 0; i < assocs.size() - 1; i++)
  //{
  //  if (assocs[i].idx1 > assocs[i + 1].idx1)
  //    countWrongs++;
  //}

  //if (countWrongs <= 1)
  //  result.first = true;

  //countWrongs = 0;
  //for (size_t i = 0; i < assocs.size() - 1; i++)
  //{
  //  if (assocs[i].idx2 > assocs[i + 1].idx2)
  //    countWrongs++;
  //}

  //if (countWrongs <= 1)
  //  result.second = true;

  //int countIdx1 = 0;
  //int countIdx2 = 0;
  //for (size_t i = 0; i < assocs.size() - 1; i++)
  //{
  //  if ((assocs[i].dist - 0.001) > limitDist)
  //    continue;

  //  if (assocs[i].idx1 == firstBorder.getFromIndex() || assocs[i].idx1 == firstBorder.getToIndex())
  //  {
  //    if (assocs[i].idx1 == firstBorder.getToIndex() && assocs[i + 1].idx1 == firstBorder.getFromIndex())
  //      countIdx1--;
  //    else if (assocs[i].idx1 == firstBorder.getFromIndex() && assocs[i + 1].idx1 == firstBorder.getToIndex())
  //      countIdx1++;
  //  }
  //  {
  //    if (assocs[i].idx1 < assocs[i + 1].idx1)
  //      countIdx1++;
  //    else if (assocs[i].idx1 != assocs[i + 1].idx1)
  //      countIdx1--;
  //  }

  //  if (assocs[i].idx2 == secondBorder.getFromIndex() || assocs[i].idx2 == secondBorder.getToIndex())
  //  {
  //    if (assocs[i].idx2 == secondBorder.getToIndex() && assocs[i + 1].idx2 == secondBorder.getFromIndex())
  //      countIdx2--;
  //    else if (assocs[i].idx2 == secondBorder.getFromIndex() && assocs[i + 1].idx2 == secondBorder.getToIndex())
  //      countIdx2++;
  //  }
  //  else
  //  {
  //    if (assocs[i].idx2 < assocs[i + 1].idx2)
  //      countIdx2++;
  //    else if (assocs[i].idx2 != assocs[i + 1].idx2)
  //      countIdx2--;
  //  }

  //  //if (assocs[i].idx1 < assocs[i + 1].idx1)
  //  //  countIdx1++;
  //  //else if (assocs[i].idx1 != assocs[i + 1].idx1)
  //  //  countIdx1--;

  //  //if (assocs[i].idx2 < assocs[i + 1].idx2)
  //  //  countIdx2++;
  //  //else if (assocs[i].idx2 != assocs[i + 1].idx2)
  //  //  countIdx2--;
  //}

  //result.first = countIdx1 >= 0;
  //result.second = countIdx2 >= 0;

  return result;
}


void GeneralBorderCalculator::sortAssociationsByIdx2(std::vector<Association>& assocs)
{
  std::unordered_map<int, std::vector<Association>> groups;

  for (size_t i = 0; i < assocs.size(); i++)
  {
    groups[assocs[i].idx1].push_back(assocs[i]);
  }

  for (auto iter = groups.begin(); iter != groups.end(); ++iter)
  {
    std::sort(iter->second.begin(), iter->second.end(),
      [](const Association& a, const Association& b) {
        return a.idx2 > b.idx2;
      });
  }

  std::vector<Association> sortedAssocs;

  for (size_t i = 0; i < assocs.size(); i++) {
    int idx1 = assocs[i].idx1;
    if (!groups[idx1].empty()) {
      sortedAssocs.insert(sortedAssocs.end(), groups[idx1].begin(), groups[idx1].end());
      groups.erase(idx1);
    }
  }

  assocs = sortedAssocs;
}



