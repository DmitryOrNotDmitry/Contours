#include "GeneralBorderCalculator.h"
#include <vector>


struct ContourBorderAttrs {

  ContourBorderAttrs(const Contour& contour, int index, int step)
    : contour(contour)
    , index(index)
    , step(step)
  {
  }

  const Contour& contour;
  int index;
  int step;
};

void calcBordersEnds(ContourBorderAttrs& contAttr1, ContourBorderAttrs& contAttr2, double maxDist)
{
  int countIter = 0;
  double dist = 0;

  while (countIter < contAttr1.contour.size())
  {
    int possibleIndex1 = contAttr1.contour.getNextIdx(contAttr1.index, contAttr1.step);
    int nearPointTo1 = contAttr2.contour.findNearestPointTo(contAttr1.contour[possibleIndex1]);

    dist = contAttr1.contour[possibleIndex1].DistanceTo(contAttr2.contour[nearPointTo1]);
    if (dist >= maxDist)
      break;

    contAttr1.index = possibleIndex1;
    countIter++;
  }

  countIter = 0;
  dist = 0;

  while (countIter < contAttr2.contour.size())
  {
    int possibleIndex2 = contAttr2.contour.getNextIdx(contAttr2.index, contAttr2.step);
    int nearPointTo2 = contAttr1.contour.findNearestPointTo(contAttr2.contour[possibleIndex2]);

    dist = contAttr2.contour[possibleIndex2].DistanceTo(contAttr1.contour[nearPointTo2]);
    if (dist >= maxDist)
      break;

    contAttr2.index = possibleIndex2;
    countIter++;
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


double calcDistDeltaByIterations(const Contour& first, const Contour& second, const std::pair<int, int>& controlPointsIndexes, bool isDiffSteps = true)
{
  int countCheckedPoints = 5;
  int step = 1;
  
  int firstIndex = controlPointsIndexes.first;
  int secondIndex = controlPointsIndexes.second;

  double delta = 0;
  for (int i = 0; i < countCheckedPoints; i++)
  {
    firstIndex = first.getNextIdx(firstIndex, step);
    if (isDiffSteps)
      secondIndex = second.getNextIdx(secondIndex, -step);
    else
      secondIndex = second.getNextIdx(secondIndex, step);

    delta += first[firstIndex].DistanceTo(second[secondIndex]);
  }
  return delta;
}


bool GeneralBorderCalculator::haveContoursSameDirection(const Contour& first, const Contour& second, const std::pair<int, int>& controlPointsIndexes)
{
  double deltaDistSameSteps = calcDistDeltaByIterations(first, second, controlPointsIndexes, false);
  double deltaDistDiffSteps = calcDistDeltaByIterations(first, second, controlPointsIndexes);

  return deltaDistSameSteps < deltaDistDiffSteps;
}


std::pair<LineBorder, LineBorder> GeneralBorderCalculator::defineNearBorders(Contour& first, Contour& second, double limitDist)
{
  std::pair<LineBorder, LineBorder> result( LineBorder(first, 0, 0), LineBorder(second, 0, 0) );
  std::pair<int, int> firstBorder;
  std::pair<int, int> secondBorder;


  std::pair<int, int> controlPoints = GeneralBorderCalculator::calculateNearestPointsIdx(first, second);

  int stepFirst = 1;
  int stepSecond = -1;
  //if (haveContoursSameDirection(first, second, controlPoints))
  //  stepSecond = 1;
  // TODO
  ContourBorderAttrs firstContAttrs(first, controlPoints.first, stepFirst);
  ContourBorderAttrs secondContAttrs(second, controlPoints.second, stepSecond);

  calcBordersEnds(firstContAttrs, secondContAttrs, limitDist);

  firstBorder.second = firstContAttrs.index;
  secondBorder.first = secondContAttrs.index;


  
  firstContAttrs.step = -stepFirst;
  secondContAttrs.step = -stepSecond;

  firstContAttrs.index = controlPoints.first;
  secondContAttrs.index = controlPoints.second;

  calcBordersEnds(firstContAttrs, secondContAttrs, limitDist);
  

  firstBorder.first = firstContAttrs.index;
  secondBorder.second = secondContAttrs.index;
  
  result.first = LineBorder(first, firstBorder.first, firstBorder.second);
  //result.first.reduceEnds(limitDistance / 2);

  result.second = LineBorder(second, secondBorder.first, secondBorder.second);
  //result.second.reduceEnds(limitDistance / 2);

  LineBorder::reduceEndsWhileApproxTo(result.first, result.second, limitDist * 2);
  
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



