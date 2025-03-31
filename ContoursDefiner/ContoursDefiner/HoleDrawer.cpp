#include "HoleDrawer.h"


void HoleDrawer::draw(HDC& hDC, double scaleX, double scaleY)
{
  if (!dataManager.isShowHoles())
    return;

  HGDIOBJ oldPen = SelectObject(hDC, pen);

  double offset = 0.25;
  double newX = 0;
  double newY = 0;

  int count_holes = dataManager.getHoles().size();
  for (size_t i = 0; i < count_holes; i++)
  {
    Contour& hole = dataManager.getHoles()[i];
    const std::vector<Point>& points = hole.getPoints();
    int numPoints = (int)points.size();
    
    if (numPoints == 0)
      continue;

    COLORREF color = dataManager.getHoleOwnerColor(hole);
    HPEN newPen = CreatePen(PS_SOLID, PEN_WIDTH, color);
    SelectObject(hDC, newPen);

    DoublePoint centerMass = massCenter(dataManager.getHoles()[i]);

    calcNewCoords(centerMass, points[0], offset, newX, newY);
    MoveToEx(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY), NULL);

    for (int j = 0; j < numPoints; j++)
    {
      calcNewCoords(centerMass, points[j], offset, newX, newY);
      LineTo(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY));
    }

    calcNewCoords(centerMass, points[0], offset, newX, newY);
    LineTo(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY));

    DeleteObject(newPen);
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
