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


void provideValideCycleIndex(const Contour& contour, int& index)
{
  int size = contour.size() - 1;
  if (index < 0)
    index += size;

  if (index > size - 1)
  {
    index %= size;
  }
}


void calcBordersEnds(ContourBorderAttrs& contAttr1, ContourBorderAttrs& contAttr2, double maxDist)
{

  while (contAttr1.contour[contAttr1.index].DistanceTo(contAttr2.contour[contAttr2.index]) < maxDist)
  {
    int possibleIndex1 = contAttr1.index + contAttr1.step;
    int possibleIndex2 = contAttr1.index + contAttr1.step * 2;
    provideValideCycleIndex(contAttr1.contour, possibleIndex1);
    provideValideCycleIndex(contAttr1.contour, possibleIndex2);

    double dist1 = contAttr1.contour[possibleIndex1].DistanceTo(contAttr2.contour[contAttr2.index]);
    double dist2 = contAttr1.contour[possibleIndex2].DistanceTo(contAttr2.contour[contAttr2.index]);

    contAttr1.index = possibleIndex1;
    if (dist1 > dist2)
      contAttr1.index = possibleIndex2;



    possibleIndex1 = contAttr2.index + contAttr2.step;
    possibleIndex2 = contAttr2.index + contAttr2.step * 2;
    provideValideCycleIndex(contAttr2.contour, possibleIndex1);
    provideValideCycleIndex(contAttr2.contour, possibleIndex2);

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
  
  const int FIRST_STEP = 10;

  int firstCurIndex = 0;
  int secondCurIndex = 0;
  
  firstCurIndex = first.findNearestPointTo(second[secondCurIndex], FIRST_STEP);
  secondCurIndex = second.findNearestPointTo(first[firstCurIndex], FIRST_STEP);


  int firstPrevIndex = firstCurIndex - 1;
  int firstNextIndex = firstCurIndex + 1;
  
  int secondPrevIndex = secondCurIndex - 1;
  int secondNextIndex = secondCurIndex + 1;

  double currentDist = 0;
  double nextDist = 0;
  double prevDist = 0;

  int step = 3;

  size_t maxSize = first.size();
  if (second.size() > maxSize)
    maxSize = second.size();

  const int max_iters = maxSize / step + 1;
  int count = 0;
  while (count < max_iters)
  {
    provideValideCycleIndex(first, firstPrevIndex);
    provideValideCycleIndex(first, firstNextIndex);
    provideValideCycleIndex(second, secondPrevIndex);
    provideValideCycleIndex(second, secondNextIndex);


    nextDist = first[firstCurIndex].DistanceTo(second[secondNextIndex]);
    prevDist = first[firstCurIndex].DistanceTo(second[secondPrevIndex]);

    if (nextDist < prevDist)
      secondCurIndex = secondNextIndex;
    else
      secondCurIndex = secondPrevIndex;


    nextDist = second[secondCurIndex].DistanceTo(first[firstNextIndex]);
    prevDist = second[secondCurIndex].DistanceTo(first[firstPrevIndex]);

    if (nextDist < prevDist)
      firstCurIndex = firstNextIndex;
    else
      firstCurIndex = firstPrevIndex;


    currentDist = first[firstCurIndex].DistanceTo(second[secondCurIndex]);
    if (currentDist < 5)
    {
      break;
    }


    firstPrevIndex = firstCurIndex - step;
    firstNextIndex = firstCurIndex + step;

    secondPrevIndex = secondCurIndex - step;
    secondNextIndex = secondCurIndex + step;

   
    count++;
  }

  firstCurIndex = first.findNearestPointTo(second[secondCurIndex], firstCurIndex - 5, firstCurIndex + 5, 1);
  secondCurIndex = second.findNearestPointTo(first[firstCurIndex], secondCurIndex - 5, secondCurIndex + 5, 1);

  result.first = firstCurIndex;
  result.second = secondCurIndex;

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
    firstIndex += step;
    if (isDiffSteps)
      secondIndex -= step;
    else
      secondIndex += step;

    provideValideCycleIndex(first, firstIndex);
    provideValideCycleIndex(second, secondIndex);

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

  LineBorder::reduceEndsWhileApproxTo(result.first, result.second);
  
  return result;
}

std::vector<Point> GeneralBorderCalculator::averageTwoLine(LineBorder first, LineBorder second)
{
  std::vector<Point> result;
  result.reserve(first.size());

  int tt = first.size();
  int tt43 = second.size();

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



