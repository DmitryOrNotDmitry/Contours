#include "ControlPointsCalculator.h"
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
  while (contAttr1.contour[contAttr1.index].DistanceTo(contAttr2.contour[contAttr2.index]) < maxDist)
  {
    int possibleIndex1 = contAttr1.contour.getNextIdx(contAttr1.index, contAttr1.step);
    int possibleIndex2 = contAttr1.contour.getNextIdx(contAttr1.index, contAttr1.step * 2);

    double dist1 = contAttr1.contour[possibleIndex1].DistanceTo(contAttr2.contour[contAttr2.index]);
    double dist2 = contAttr1.contour[possibleIndex2].DistanceTo(contAttr2.contour[contAttr2.index]);

    contAttr1.index = possibleIndex1;
    if (dist1 > dist2)
      contAttr1.index = possibleIndex2;


    possibleIndex1 = contAttr2.contour.getNextIdx(contAttr2.index, contAttr2.step);
    possibleIndex2 = contAttr2.contour.getNextIdx(contAttr2.index, contAttr2.step * 2);

    dist1 = contAttr2.contour[possibleIndex1].DistanceTo(contAttr1.contour[contAttr1.index]);
    dist2 = contAttr2.contour[possibleIndex2].DistanceTo(contAttr1.contour[contAttr1.index]);

    contAttr2.index = possibleIndex1;
    if (dist1 > dist2)
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

  while (dist > 5)
  {
    firstIndex += (int)floor(dist) % first.size();
    secondNearPointIdx = second.findNearestPointTo(first[firstIndex]);

    dist = first[firstIndex].DistanceTo(second[secondNearPointIdx]);
  }

  result.first = firstIndex;
  result.second = secondNearPointIdx;

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


std::pair<LineBorder, LineBorder> GeneralBorderCalculator::defineNearBorders(Contour& first, Contour& second)
{
  std::pair<LineBorder, LineBorder> result( LineBorder(first, 0, 0), LineBorder(second, 0, 0) );
  std::pair<int, int> firstBorder;
  std::pair<int, int> secondBorder;


  std::pair<int, int> controlPoints = GeneralBorderCalculator::calculateNearestPointsIdx(first, second);
  
  const int limitDistance = 6;

  int stepFirst = 1;
  int stepSecond = -1;
  //if (haveContoursSameDirection(first, second, controlPoints))
  //  stepSecond = 1;
  // TODO
  ContourBorderAttrs firstContAttrs(first, controlPoints.first, stepFirst);
  ContourBorderAttrs secondContAttrs(second, controlPoints.second, stepSecond);

  calcBordersEnds(firstContAttrs, secondContAttrs, limitDistance);

  firstBorder.second = firstContAttrs.index;
  secondBorder.first = secondContAttrs.index;



  firstContAttrs.step = -stepFirst;
  secondContAttrs.step = -stepSecond;

  firstContAttrs.index = controlPoints.first;
  secondContAttrs.index = controlPoints.second;

  calcBordersEnds(firstContAttrs, secondContAttrs, limitDistance);

  firstBorder.first = firstContAttrs.index;
  secondBorder.second = secondContAttrs.index;

  result.first = LineBorder(first, firstBorder.first, firstBorder.second);
  //result.first.reduceEnds(limitDistance / 2);

  result.second = LineBorder(second, secondBorder.first, secondBorder.second);
  //result.second.reduceEnds(limitDistance / 2);

  LineBorder::reduceEndsWhileApproxTo(result.first, result.second, 10);
  
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



