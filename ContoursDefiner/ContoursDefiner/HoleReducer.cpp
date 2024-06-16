#include "HoleReducer.h"
#include "LineBorderVector.h"
#include "GeneralBorderCalculator.h"
#include "DataStorageManager.h"

#include <map>
#include <set>


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


Contour* getContourWithMaxBorder(Contour& hole, std::list<Contour>& contours)
{
  std::vector<Contour*> contsWithGeneralBorder = hole.calcNeighbors(contours);

  Contour* contWithMaxBorder = nullptr;
  double maxBorderSize = 0;
  double limitDistance = 1;

  for (size_t i = 0; i < contsWithGeneralBorder.size(); i++)
  {
    auto borders = GeneralBorderCalculator::defineNearBorders(hole, *contsWithGeneralBorder[i], limitDistance);
    
    double curSize = borders.first.euclideanLength();
    
    if (curSize > maxBorderSize)
    {
      contWithMaxBorder = contsWithGeneralBorder[i];
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

  Contour* selectedCont = getContourWithMaxBorder(hole, contours);
  if (selectedCont)
  {
    auto borders = GeneralBorderCalculator::defineNearBorders(hole, *selectedCont, limitDistance);

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


void reduceHoleMultiBorders(Contour& hole, std::list<Contour>& contours)
{
  double limitDistance = 1;
  std::vector<Contour*> contourCandiates = getContourSortedByBorderSize(hole, contours);

  for (size_t i = 0; i < contourCandiates.size(); i++)
  {
    if (hole.area() < 1.001)
    {
      includeIntoDominant(hole, contours);
      return;
    }

    Contour* selectedCont = contourCandiates[i];

    std::vector<Contour*> checkOnValideContour = hole.calcNeighbors(contours);
    if (std::find(checkOnValideContour.begin(), checkOnValideContour.end(), selectedCont) == checkOnValideContour.end())
      continue;
    
    auto borders = GeneralBorderCalculator::defineNearBorders(hole, *selectedCont, limitDistance);

    if (borders.second.size() <= 1)
      continue;

    std::vector<Point> newBorder = defineNewBorder(hole, *selectedCont, borders.second);

    if (newBorder.size() > 0)
    {
      LineBorderVector lineBorder(newBorder);

      borders.second.replaceBorderWith(lineBorder);
      borders.first.replaceBorderWith(lineBorder);
    
      if (hole.area() < 0.001)
        return;

      hole.deletePins();
    }
  }
  
  std::vector<Contour> sepHoles = hole.separate();

  for (size_t i = 0; i < sepHoles.size(); i++)
    if (sepHoles[i].area() > 0.001)
      reduceHoleMultiBorders(sepHoles[i], contours);
    
}


//void HoleReducer::process(Contour& hole, std::list<Contour>& contours)
//{
//  reduceHole(hole, contours);
//}

void HoleReducer::processMulti(Contour& hole, std::list<Contour>& contours)
{
  reduceHoleMultiBorders(hole, contours);
}
