#include "ControlPointsCalculator.h"
#include <vector>


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


std::pair<std::pair<int, int>, std::pair<int, int>> GeneralBorderCalculator::defineGeneralBorders(const Contour& first, const Contour& second)
{
  std::pair<std::pair<int, int>, std::pair<int, int>> result;
  std::pair<int, int> firstBorder;
  std::pair<int, int> secondBorder;
  firstBorder.first = first.size() - 10;
  firstBorder.second = 10;
  secondBorder.first = second.size() - 10;
  secondBorder.second = 10;


  std::pair<int, int> controlPoints = GeneralBorderCalculator::calculateNearestPointsIdx(first, second);
  
  int stepFirst = 1;
  int stepSecond = -1;
  if (haveContoursSameDirection(first, second, controlPoints))
    stepSecond = 1;

  int firstIndex = controlPoints.first;
  int secondIndex = controlPoints.second;

  while (first[firstIndex].DistanceTo(second[secondIndex]) < 10)
  {
    firstIndex += stepFirst;
    secondIndex += stepSecond;
    provideValideCycleIndex(first, firstIndex);
    provideValideCycleIndex(second, secondIndex);
  }

  firstBorder.second = firstIndex;
  secondBorder.first = secondIndex;



  stepFirst = -stepFirst;
  stepSecond = -stepSecond;

  firstIndex = controlPoints.first;
  secondIndex = controlPoints.second;

  while (first[firstIndex].DistanceTo(second[secondIndex]) < 10)
  {
    firstIndex += stepFirst;
    secondIndex += stepSecond;
    provideValideCycleIndex(first, firstIndex);
    provideValideCycleIndex(second, secondIndex);
  }

  firstBorder.first = firstIndex;
  secondBorder.second = secondIndex;


  result.first = firstBorder;
  result.second = secondBorder;
  return result;
}
