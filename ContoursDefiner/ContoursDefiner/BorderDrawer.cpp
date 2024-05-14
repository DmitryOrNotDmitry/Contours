#include "BorderDrawer.h"

void BorderDrawer::draw(HDC& hDC, double scaleX, double scaleY)
{
  HGDIOBJ oldPen = SelectObject(hDC, pen);

  std::vector<LineBorder>& borders = dataManager.getBorders();
  for (size_t i = 0; i < borders.size(); i++)
  {
    if (borders[i].getOwner().getState() == HIDDEN)
      continue;

    const std::vector<Point>& points = borders[i].getOwner().getPoints();
    int numPoints = (int)points.size();

    int from = borders[i].getFromIndex();
    int to = borders[i].getToIndex();
    int size = borders[i].size();

    MoveToEx(hDC, toFloatDraw(points[from].x, scaleX), toFloatDraw(points[from].y, scaleY), NULL);
    for (int j = 0; j < size; j++)
    {
      int idx = borders[i].getNextIdx(borders[i].getFromIndex(), j);
      LineTo(hDC, toFloatDraw(points[idx].x, scaleX), toFloatDraw(points[idx].y, scaleY));
    }
  }

  SelectObject(hDC, oldPen);
}
