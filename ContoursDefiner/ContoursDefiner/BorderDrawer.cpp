#include "BorderDrawer.h"

void BorderDrawer::draw(HDC& hDC, double scaleX, double scaleY)
{
  HGDIOBJ oldPen = SelectObject(hDC, pen);

  std::vector<LineBorder>& borders = dataManager.getBorders();
  for (size_t i = 0; i < borders.size(); i++)
  {
    LineBorder& border = borders[i];
    if (border.getOwner().getState() == HIDDEN)
      continue;

    int size = border.size();

    MoveToEx(hDC, toFloatDraw(border.fromPoint().x, scaleX), toFloatDraw(border.fromPoint().y, scaleY), NULL);
    for (int j = 0; j < size; j++)
    {
      int idx = border.getNextIdx(border.getFromIndex(), j);
      LineTo(hDC, toFloatDraw(border.getPoint(idx).x, scaleX), toFloatDraw(border.getPoint(idx).y, scaleY));
    }
  }

  SelectObject(hDC, oldPen);
}
