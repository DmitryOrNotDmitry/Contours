#include "ContourDrawer.h"

void ContourDrawer::drawLines(HDC& hDC, double scaleX, double scaleY, ContourState state)
{
  std::list<Contour>& contours = dataManager.getContours();

  for (auto iter = contours.begin(); iter != contours.end(); ++iter)
  {
    if (dataManager.getContourState(*iter) != state)
      continue;

    size_t numPoints = iter->size();

    if (numPoints < 1)
      continue;

    std::vector<Point>& points = iter->getPoints();

    MoveToEx(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY), NULL);
    for (size_t j = 1; j < numPoints; j++)
    {
      LineTo(hDC, toFloatDraw(points[j].x, scaleX), toFloatDraw(points[j].y, scaleY));
    }
    LineTo(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY));
  }
}

void ContourDrawer::drawPoints(HDC& hDC, double scaleX, double scaleY, ContourState state)
{
  std::list<Contour>& contours = dataManager.getContours();

  for (auto iter = contours.begin(); iter != contours.end(); ++iter)
  {
    if (dataManager.getContourState(*iter) != state)
      continue;

    size_t numPoints = iter->size();

    if (numPoints < 1)
      continue;

    std::vector<Point>& points = iter->getPoints();

    const int offset = 5;
    int x, y;
    
    for (size_t j = 0; j < numPoints; j++)
    {
      x = toFloatDraw(points[j].x, scaleX);
      y = toFloatDraw(points[j].y, scaleY);
      Ellipse(hDC, x - offset, y - offset, x + offset, y + offset);
    }
  }
}

void ContourDrawer::draw(HDC& hDC, double scaleX, double scaleY)
{
  HGDIOBJ oldPen = SelectObject(hDC, visiblePen);
  HGDIOBJ oldBrush = SelectObject(hDC, visibleBrush);
  drawPoints(hDC, scaleX, scaleY, VISIBLE);
  drawLines(hDC, scaleX, scaleY, VISIBLE);

  SelectObject(hDC, selectedPen);
  SelectObject(hDC, selectedBrush);
  drawPoints(hDC, scaleX, scaleY, SELECTED);
  drawLines(hDC, scaleX, scaleY, SELECTED);

  SelectObject(hDC, oldPen);
  SelectObject(hDC, oldBrush);
}


