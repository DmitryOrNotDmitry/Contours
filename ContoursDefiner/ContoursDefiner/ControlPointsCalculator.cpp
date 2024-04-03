#include "ControlPointsCalculator.h"
#include <vector>


void provideValideCycleIndex(const Contour& contour, int& index)
{
  int size = contour.size() - 1;
  if (index < 0)
    index += size;

  if (index > size - 1)
  {
    index += size;
    index %= size;
  }
}


std::pair<Point, Point> ControlPointsCalculator::calculate(const Contour& first, const Contour& second)
{
  std::pair<Point, Point> result;
  result.first = Point(1, 1);
  result.second = Point(2, 2);

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

  result.first = first[firstCurIndex];
  result.second = second[secondCurIndex];

  return result;
}


//std::pair<std::pair<int, int>, std::pair<int, int>> ControlPointsCalculator::defineGeneralBorders(const Contour& first, const Contour& second)
//{
//  std::pair<std::pair<int, int>, std::pair<int, int>> result;
//  std::pair<int, int> firstBorder;
//  std::pair<int, int> secondBorder;
//  firstBorder.first = 0;
//  firstBorder.second = 10;
//  secondBorder.first = 0;
//  secondBorder.second = 10;
//
//
//  result.first = firstBorder;
//  result.second = secondBorder;
//  return result;
//}


