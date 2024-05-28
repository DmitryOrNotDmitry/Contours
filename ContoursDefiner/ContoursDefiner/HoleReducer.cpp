#include "HoleReducer.h"
#include "LineBorderVector.h"
#include "GeneralBorderCalculator.h"
#include "DataStorageManager.h"


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
    if (borders.first.euclideanLength() > maxBorderSize)
    {
      contWithMaxBorder = contsWithGeneralBorder[i];
      maxBorderSize = borders.first.euclideanLength();
    }
  }

  return contWithMaxBorder;
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

    for (size_t j = 0; j < checkedPoints.size(); j++)
      addPointWithCondition(newBorder, hole, contour, p, checkedPoints[j]);
  }

  return newBorder;
}


void reduceHole(Contour& hole, std::list<Contour>& contours)
{
  double limitDistance = 1;

  Contour* selectedCont = getContourWithMaxBorder(hole, contours);

  if (selectedCont)
  {
    auto borders = GeneralBorderCalculator::defineNearBorders(hole, *selectedCont, limitDistance);

    std::vector<Point> newBorder = defineNewBorder(hole, *selectedCont, borders.second);

    LineBorderVector lineBorder(newBorder);

    borders.second.replaceBorderWith(lineBorder);
    borders.first.replaceBorderWith(lineBorder);

    if (hole.area() > 0.001)
      hole.deletePins();
   
    std::vector<Contour> sepHoles = hole.separate();

    for (size_t i = 0; i < sepHoles.size(); i++)
      if (sepHoles[i].area() > 0.001)
        reduceHole(sepHoles[i], contours);
  }
}


void HoleReducer::process(Contour& hole, std::list<Contour>& contours)
{
  reduceHole(hole, contours);
}
