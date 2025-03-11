#include "HoleDrawer.h"


std::pair<double, double> massCenter(Contour contour) {
  const std::vector<Point>& points = contour.getPoints();

  double cx = 0.0, cy = 0.0;
  double area_sum = 0.0;

  for (size_t i = 0; i < points.size(); ++i) {
    size_t j = (i + 1) % points.size();
    double cross = points[i].x * points[j].y - points[j].x * points[i].y;
    cx += (points[i].x + points[j].x) * cross;
    cy += (points[i].y + points[j].y) * cross;
    area_sum += cross;
  }

  area_sum *= 0.5;
  if (area_sum == 0.0) {
    area_sum = 1;
  }

  cx /= (6.0 * area_sum);
  cy /= (6.0 * area_sum);

  return std::make_pair(cx, cy);
}


void calcNewCoords(std::pair<double, double> massCenter, Point point, double factor, double& x, double& y)
{
  x = point.x + factor * (massCenter.first - point.x);
  y = point.y + factor * (massCenter.second - point.y);
}


void HoleDrawer::draw(HDC& hDC, double scaleX, double scaleY)
{
  if (!dataManager.isShowHoles())
    return;

  HGDIOBJ oldPen = SelectObject(hDC, pen);

  double factor = 0.4;
  double newX = 0;
  double newY = 0;

  int count_holes = dataManager.getHoles().size();
  for (size_t i = 0; i < count_holes; i++)
  {
    const std::vector<Point>& points = dataManager.getHoles()[i].getPoints();
    int numPoints = (int)points.size();
    
    if (numPoints == 0)
      continue;

    std::pair<double, double> centerMass = massCenter(dataManager.getHoles()[i]);

    calcNewCoords(centerMass, points[0], factor, newX, newY);
    MoveToEx(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY), NULL);

    for (int j = 0; j < numPoints; j++)
    {
      calcNewCoords(centerMass, points[j], factor, newX, newY);
      LineTo(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY));
    }

    calcNewCoords(centerMass, points[0], factor, newX, newY);
    LineTo(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY));
  }

  SelectObject(hDC, oldPen);



  //HGDIOBJ oldPen = SelectObject(hDC, pen);

  //int count_holes = dataManager.getHoles().size();
  //for (size_t i = 0; i < count_holes; i++)
  //{
  //  const std::vector<Point>& points = dataManager.getHoles()[i].getPoints();
  //  int numPoints = (int)points.size();

  //  for (int j = 0; j < numPoints; j++)
  //  {
  //    MoveToEx(hDC, toFloatDraw(points[j].x, scaleX), toFloatDraw(points[j].y, scaleY), NULL);
  //    LineTo(hDC, toFloatDraw(points[j].x, scaleX), toFloatDraw(points[j].y, scaleY));
  //  }
  //}

  //SelectObject(hDC, oldPen);
}
