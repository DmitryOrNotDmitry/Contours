#include "HoleReducer.h"
#include "LineBorderVector.h"
#include "GeneralBorderCalculator.h"
#include "DataStorageManager.h"
#include "HoleSeparator.h"


#define EPSILON 0.001


HoleReducer::HoleReducer(double minSquare, double maxSquare)
  : minSquare(minSquare)
  , maxSquare(maxSquare)
  , limitDistance(0)
{
  reducedHole = nullptr;
}


void HoleReducer::prepareContours(std::list<Contour>& contours)
{
  nearbyContours = reducedHole->calcNeighbors(contours);

  std::sort(nearbyContours.begin(), nearbyContours.end(),
    [&](Contour* a, Contour* b) -> bool {
      std::pair<LineBorder, LineBorder> aBorders = GeneralBorderCalculator::defineNearBorders(*reducedHole, *a, limitDistance);
      std::pair<LineBorder, LineBorder> bBorders = GeneralBorderCalculator::defineNearBorders(*reducedHole, *b, limitDistance);

      return aBorders.first.size() > bBorders.first.size();
    }
  );
}


Contour* HoleReducer::getContourWithMaxBorder(Contour& hole, std::vector<Contour*> contours)
{
  Contour* contWithMaxBorder = nullptr;
  double maxBorderSize = 0;

  for (size_t i = 0; i < contours.size(); i++)
  {
    auto borders = GeneralBorderCalculator::defineNearBorders(hole, *contours[i], limitDistance);
    
    double curSize = borders.first.euclideanLength();
    
    if (curSize > maxBorderSize)
    {
      contWithMaxBorder = contours[i];
      maxBorderSize = curSize;
    }
  }

  return contWithMaxBorder;
}


void HoleReducer::includeIntoDominant(Contour& hole)
{
  Contour* selectedCont = getContourWithMaxBorder(hole, nearbyContours);
  if (selectedCont)
  {
    auto borders = GeneralBorderCalculator::defineNearBorders(hole, *selectedCont, limitDistance);

    if (borders.second.size() <= 1)
      return;

    LineBorder newBorder = borders.first.inverse();

    borders.second.replaceBorderWith(newBorder);
    borders.first.replaceBorderWith(borders.second);
  }

}


void HoleReducer::reduceHoleMultiBorders()
{
  Contour& hole = *reducedHole;
  
  std::vector<Contour> atomicHoles = HoleSeparator::separateToAtomicParts(hole);
  
  for (size_t i = 0; i < nearbyContours.size(); i++)
  {
    countHolesForContour.insert(std::make_pair(nearbyContours[i], 0));
  }

  while (atomicHoles.size() > 0)
  {
    for (auto iter = countHolesForContour.begin(); iter != countHolesForContour.end(); ++iter)
    {
      iter->second = 0;
    }

    distributeHolesToContours(atomicHoles);

    if (holesDistribution.size() == 0)
      break;

    defineSkippedContours(atomicHoles);

    for (auto iter = holesDistribution.rbegin(); iter != holesDistribution.rend(); ++iter)
    {
      Contour& hole = *iter->second.hole;
      Contour& selectedCont = *iter->second.forContour;
      
      if (skippedContours.find(&selectedCont) != skippedContours.end())
        continue;

      auto borders = GeneralBorderCalculator::defineNearBorders(hole, selectedCont, limitDistance);

      LineBorder newBorder = borders.first.inverse();

      borders.second.replaceBorderWith(newBorder);
      borders.first.replaceBorderWith(borders.second);
    }

    for (size_t i = 0; i < nearbyContours.size(); i++)
    {
      nearbyContours[i]->deletePins();
    }

    std::vector<Contour> restHoles;
    restHoles.reserve(atomicHoles.size());
    for (size_t i = 0; i < atomicHoles.size(); i++)
      if (atomicHoles[i].area() > EPSILON)
        restHoles.push_back(std::move(atomicHoles[i]));

    atomicHoles = std::move(restHoles);
  }
}


void HoleReducer::distributeHolesToContours(std::vector<Contour>& holes)
{
  holesDistribution.clear();
  for (size_t i = 0; i < holes.size(); i++)
  {
    Contour* selectedCont = getContourWithMaxBorder(holes[i], nearbyContours);

    if (selectedCont == nullptr)
      continue;

    auto borders = GeneralBorderCalculator::defineNearBorders(holes[i], *selectedCont, limitDistance);

    if (borders.second.size() <= 1)
      continue;

    int len = static_cast<int>(borders.first.squareLength());

    countHolesForContour[selectedCont]++;
    holesDistribution.insert(std::make_pair(len, HoleDistribution(&holes[i], selectedCont)));
  }
}


void HoleReducer::defineSkippedContours(std::vector<Contour>& holes)
{
  skippedContours.clear();
  if (holesDistribution.size() < holes.size())
  {
    for (auto iter = countHolesForContour.begin(); iter != countHolesForContour.end(); ++iter)
      if (iter->second <= 1)
        skippedContours.insert(iter->first);
  }
}


void HoleReducer::processMulti(Contour& hole, std::list<Contour>& contours)
{
  reducedHole = &hole;

  double holeArea = hole.area();
  
  if (holeArea > (maxSquare - EPSILON))
  {
    return;
  }

  prepareContours(contours);

  if (holeArea < (minSquare + EPSILON))
  {
    includeIntoDominant(hole);
    return;
  }

  reduceHoleMultiBorders();
}



// DEPRECATED
std::vector<Point> defineNewBorder(Contour& hole, Contour& contour, LineBorder oldBorder);
void addPointWithCondition(std::vector<Point>& newBorder,
  const Contour& hole,
  const Contour& contour,
  Point basePoint,
  Point p);
void sortPointsByProximity(std::vector<Point>& points, const Point& anchorPoint);

void HoleReducer::process(Contour& hole, std::list<Contour>& contours)
{
  reducedHole = &hole;

  double holeArea = hole.area();

  if (holeArea > (maxSquare - EPSILON))
  {
    return;
  }

  prepareContours(contours);

  reduceHole(hole, contours);
}

void HoleReducer::reduceHole(Contour& hole, std::list<Contour>& contours)
{
  if (hole.area() < (minSquare + EPSILON))
  {
    includeIntoDominant(hole);
    return;
  }

  Contour* selectedCont = getContourWithMaxBorder(hole, nearbyContours);

  if (selectedCont)
  {
    int countPointsGiven = givePartHoleToContour(hole, *selectedCont);
    if (countPointsGiven == 0)
      return;
    
    if (hole.area() > EPSILON)
      hole.deletePins();

    std::vector<Contour> sepHoles = hole.separate();
    
    for (size_t i = 0; i < sepHoles.size(); i++)
      if (sepHoles[i].area() > EPSILON)
        reduceHole(sepHoles[i], contours);
  }
}

int HoleReducer::givePartHoleToContour(Contour& hole, Contour& contour)
{
  auto borders = GeneralBorderCalculator::defineNearBorders(hole, contour, limitDistance);

  std::vector<Point> newBorder = defineNewBorder(hole, contour, borders.second);

  if (newBorder.size() == 0)
    return 0;

  LineBorderVector lineBorder(newBorder);

  borders.second.replaceBorderWith(lineBorder);
  borders.first.replaceBorderWith(lineBorder);

  return newBorder.size();
}

std::vector<Point> defineNewBorder(Contour& hole, Contour& contour, LineBorder oldBorder)
{
  std::vector<Point> newBorder;

  int countIters = oldBorder.size();
  int index = oldBorder.getFromIndex();
  std::vector<Point> checkedPoints;
  checkedPoints.resize(4);

  for (int i = 0; i < countIters; i++)
  {
    Point p = oldBorder.getPoint(index);
    index = oldBorder.getNextIdx(index, 1);

    checkedPoints[0] = p.toRight();
    checkedPoints[1] = p.toBottom();
    checkedPoints[2] = p.toLeft();
    checkedPoints[3] = p.toUp();

    if (newBorder.size() > 0)
      sortPointsByProximity(checkedPoints, *newBorder.rbegin());
    else
      sortPointsByProximity(checkedPoints, oldBorder.fromPoint());


    for (size_t j = 0; j < checkedPoints.size(); j++)
      addPointWithCondition(newBorder, hole, contour, p, checkedPoints[j]);
  }

  return newBorder;
}

void addPointWithCondition(std::vector<Point>& newBorder,
  const Contour& hole,
  const Contour& contour,
  Point basePoint,
  Point p)
{
  if ((hole.isInner(p) || hole.contains(p)) && !contour.contains(p))
  {
    if (std::find(newBorder.begin(), newBorder.end(), p) == newBorder.end())
    {
      if ((newBorder.size() > 0) && (newBorder.rbegin()->DistanceTo(p) >= 1.999))
        newBorder.push_back(basePoint);

      newBorder.push_back(p);
    }
  }
}

void sortPointsByProximity(std::vector<Point>& points, const Point& anchorPoint) 
{
  std::sort(points.begin(), points.end(),
    [&](const Point& a, const Point& b) {
      return anchorPoint.DistanceTo(a) < anchorPoint.DistanceTo(b);
    }
  );
}

// DEPRECATED - end
