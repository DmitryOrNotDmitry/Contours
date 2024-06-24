#include "HoleReducer.h"
#include "LineBorderVector.h"
#include "GeneralBorderCalculator.h"
#include "DataStorageManager.h"
#include "HoleSeparator.h"

#include <map>
#include <set>
#include <functional>

#define EPSILON 0.001


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


Contour* getContourWithMaxBorder(Contour& hole, std::vector<Contour*> contours)
{
  Contour* contWithMaxBorder = nullptr;
  double maxBorderSize = 0;
  double limitDistance = 1;

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


std::vector<Contour*> getContourSortedByBorderSize(Contour& hole, std::list<Contour>& contours)
{
  std::vector<Contour*> contsWithGeneralBorder = hole.calcNeighbors(contours);
  double limitDistance = 1;

  std::map<double, Contour*> borderLenToCont;

  for (size_t i = 0; i < contsWithGeneralBorder.size(); i++)
  {
    Contour* c = contsWithGeneralBorder[i];

    auto borders = GeneralBorderCalculator::defineNearBorders(hole, *c, limitDistance);
    double curLen = borders.second.euclideanLength();

    borderLenToCont[curLen] = c;
  }

  contsWithGeneralBorder.clear();
  
  for (auto iter = borderLenToCont.rbegin(); iter != borderLenToCont.rend(); ++iter)
  {
    contsWithGeneralBorder.push_back(iter->second);
  }

  return contsWithGeneralBorder;
}


void sortPointsByProximity(std::vector<Point>& points, const Point& anchorPoint) {

  std::sort(points.begin(), points.end(),
    [&](const Point& a, const Point& b) {
      return anchorPoint.DistanceTo(a) < anchorPoint.DistanceTo(b);
    }
  );

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


int givePartHoleToContour(Contour& hole, Contour& contour)
{
  double limitDistance = 1;
  
  auto borders = GeneralBorderCalculator::defineNearBorders(hole, contour, limitDistance);

  std::vector<Point> newBorder = defineNewBorder(hole, contour, borders.second);

  if (newBorder.size() == 0)
    return 0;

  LineBorderVector lineBorder(newBorder);

  borders.second.replaceBorderWith(lineBorder);
  borders.first.replaceBorderWith(lineBorder);

  return newBorder.size();
}


void includeIntoDominant(Contour& hole, std::list<Contour>& contours)
{
  double limitDistance = 1;

  std::vector<Contour*> contsWithGeneralBorder = hole.calcNeighbors(contours);

  Contour* selectedCont = getContourWithMaxBorder(hole, contsWithGeneralBorder);
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


//void reduceHole(Contour& hole, std::list<Contour>& contours)
//{
  //if (hole.area() < 1.001)
  //{
  //  includeIntoDominant(hole, contours);
  //  return;
  //}

  //double limitDistance = 1;

  //Contour* selectedCont = getContourWithMaxBorder(hole, contours);

  //if (selectedCont)
  //{
  //  int countPointsGiven = givePartHoleToContour(hole, *selectedCont);
  //  if (countPointsGiven == 0)
  //    return;

  //  if (hole.area() > 0.001)
  //    hole.deletePins();

  //  selectedCont->haveRepeatPoints();

  //  std::vector<Contour> sepHoles = hole.separate();

  //  for (size_t i = 0; i < sepHoles.size(); i++)
  //    if (sepHoles[i].area() > 0.001)
  //      reduceHole(sepHoles[i], contours);
  //}
//}



struct HoleDistribution
{
  HoleDistribution() : hole(nullptr), forContour(nullptr) {}

  HoleDistribution(Contour* hole, Contour* forContour)
    : hole(hole), forContour(forContour) { }
     
  Contour* hole;
  Contour* forContour;
};


void reduceHoleMultiBorders(Contour& hole, std::list<Contour>& contours)
{
  const double limitDistance = 1;

  std::vector<Contour*> contsWithGeneralBorder = hole.calcNeighbors(contours);

  std::sort(contsWithGeneralBorder.begin(), contsWithGeneralBorder.end(),
    [&](Contour* a, Contour* b) -> bool {
      std::pair<LineBorder, LineBorder> aBorders = GeneralBorderCalculator::defineNearBorders(hole, *a, limitDistance);
      std::pair<LineBorder, LineBorder> bBorders = GeneralBorderCalculator::defineNearBorders(hole, *b, limitDistance);

      return aBorders.first.size() > bBorders.second.size();
    }
  );
  
  std::vector<Contour> atomicHoles = HoleSeparator::separateToAtomicParts(hole);
  std::multimap<int, HoleDistribution> holesDistribution;

  //std::map<Contour*, int> holesForContour;
  //for (size_t i = 0; i < contsWithGeneralBorder.size(); i++)
  //{
  //  holesForContour.insert(std::make_pair(contsWithGeneralBorder[i], 0));
  //}

  while (atomicHoles.size() > 0)
  {
    holesDistribution.clear();
    //for (auto iter = holesForContour.begin(); iter != holesForContour.end(); ++iter)
    //{
    //  iter->second = 0;
    //}

    for (size_t i = 0; i < atomicHoles.size(); i++)
    {
      Contour* selectedCont = getContourWithMaxBorder(atomicHoles[i], contsWithGeneralBorder);

      if (selectedCont == nullptr)
        continue;

      if (atomicHoles[i].contains(Point(63, 28)) && atomicHoles[i].contains(Point(63, 29))) // TODO
        int t = 0;
      if (atomicHoles[i].contains(Point(65, 28)) && atomicHoles[i].contains(Point(65, 29))) // TODO
        int t = 0;
      if (atomicHoles[i].contains(Point(65, 29)) && atomicHoles[i].contains(Point(65, 30))) // TODO
        int t = 0;

      auto borders = GeneralBorderCalculator::defineNearBorders(atomicHoles[i], *selectedCont, limitDistance);   

      if (borders.second.size() <= 1)
        continue;

      int len = static_cast<int>(borders.first.squareLength());

      //holesForContour[selectedCont]++;
      holesDistribution.insert(std::make_pair(len, HoleDistribution(&atomicHoles[i], selectedCont)));
    }


    if (holesDistribution.size() == 0)
      break;


    //for (auto iter = holesForContour.begin(); iter != holesForContour.end(); ++iter)
    //{
    //  if (iter->second == 1)
    //    int t = 0;
    //}



    for (auto iter = holesDistribution.rbegin(); iter != holesDistribution.rend(); ++iter)
    {
      Contour& hole = *iter->second.hole;
      Contour& selectedCont = *iter->second.forContour;

      auto borders = GeneralBorderCalculator::defineNearBorders(hole, selectedCont, limitDistance);

      double len = iter->first;

      if (borders.first.size() != borders.second.size())
        int y = 0;


      if (hole.contains(Point(63, 28)) && hole.contains(Point(63, 29))) // TODO
        int t = 0;
      if (hole.contains(Point(65, 28)) && hole.contains(Point(65, 29))) // TODO
        int t = 0;
      if (hole.contains(Point(65, 29)) && hole.contains(Point(65, 30))) // TODO
        int t = 0;

     /* if (borders.first.size() >= 5 || borders.second.size() >= 5)
        continue;*/

      LineBorder newBorder = borders.first.inverse();

      borders.second.replaceBorderWith(newBorder);
      borders.first.replaceBorderWith(borders.second);
    }

    for (size_t i = 0; i < contsWithGeneralBorder.size(); i++)
    {
      contsWithGeneralBorder[i]->deletePins();
    }

    //break; // TODO

    std::vector<Contour> restHoles;
    restHoles.reserve(atomicHoles.size());
    for (size_t i = 0; i < atomicHoles.size(); i++)
      if (atomicHoles[i].area() > 0.001)
        restHoles.push_back(std::move(atomicHoles[i]));

    atomicHoles = std::move(restHoles);
  }
}


//void HoleReducer::process(Contour& hole, std::list<Contour>& contours)
//{
//  reduceHole(hole, contours);
//}

void HoleReducer::processMulti(Contour& hole, std::list<Contour>& contours, double minSquare, double maxSquare)
{
  double holeArea = hole.area();

  if (holeArea > (maxSquare - EPSILON))
  {
    return;
  }

  if (holeArea < (minSquare + EPSILON))
  {
    includeIntoDominant(hole, contours);
    return;
  }

  reduceHoleMultiBorders(hole, contours);
}
