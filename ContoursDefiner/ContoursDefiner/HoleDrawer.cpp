#include "HoleDrawer.h"

void HoleDrawer::draw(HDC& hDC, double scaleX, double scaleY)
{
  HGDIOBJ oldPen = SelectObject(hDC, pen);

  int count_holes = dataManager.getHoles().size();
  for (size_t i = 0; i < count_holes; i++)
  {
    const std::vector<Point>& points = dataManager.getHoles()[i].getPoints();
    int numPoints = (int)points.size();

    MoveToEx(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY), NULL);
    for (int j = 0; j < numPoints; j++)
    {
      LineTo(hDC, toFloatDraw(points[j].x, scaleX), toFloatDraw(points[j].y, scaleY));
    }
    LineTo(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY));
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
