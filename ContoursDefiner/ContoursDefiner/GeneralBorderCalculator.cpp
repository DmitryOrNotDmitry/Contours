#include "GeneralBorderCalculator.h"
#include <vector>


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
  std::pair<LineBorder, LineBorder> result( LineBorder(first, 0, 0), LineBorder(second, 0, 0) );
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

  /*if (result.first.size() == result.first.getOwner().size())
  {
    if (result.first.isClockwise() != result.second.isClockwise())
      result.first.getOwner().reverse();

    result.first.agreeWith(result.second);
  }*/

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



