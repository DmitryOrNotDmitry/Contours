#include "ContourDrawer.h"

void ContourDrawer::drawWithState(HDC& hDC, double scaleX, double scaleY, ContourState state)
{
  std::vector<Contour>& contours = dataManager.getContours();
  for (size_t i = 0; i < contours.size(); i++)
  {
    if (contours[i].getState() != state)
      continue;

    size_t numPoints = contours[i].size();

    if (numPoints < 1)
      continue;

    std::vector<Point>& points = contours[i].getPoints();

    MoveToEx(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY), NULL);
    for (size_t j = 1; j < numPoints; j++)
    {
      LineTo(hDC, toFloatDraw(points[j].x, scaleX), toFloatDraw(points[j].y, scaleY));
    }
    LineTo(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY));
  }
}

void ContourDrawer::draw(HDC& hDC, double scaleX, double scaleY)
{
  HGDIOBJ oldPen = SelectObject(hDC, visiblePen);
  drawWithState(hDC, scaleX, scaleY, VISIBLE);

  SelectObject(hDC, selectedPen);
  drawWithState(hDC, scaleX, scaleY, SELECTED);

  SelectObject(hDC, oldPen);
}


