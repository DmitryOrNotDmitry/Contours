#include "ContourDrawer.h"
#include <map>

void ContourDrawer::drawLines(HDC& hDC, double scaleX, double scaleY, Contour& contour)
{
  double offset = 0.0;
  double newX = 0;
  double newY = 0;

  size_t numPoints = contour.size();

  if (numPoints < 1)
    return;

  std::vector<Point>& points = contour.getPoints();

  DoublePoint centerMass = massCenter(contour);
   
  calcNewCoords(centerMass, points[0], offset, newX, newY);
  MoveToEx(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY), NULL);
    
  for (size_t j = 1; j < numPoints; j++)
  {
    calcNewCoords(centerMass, points[j], offset, newX, newY);
    LineTo(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY)); 
  }
  calcNewCoords(centerMass, points[0], offset, newX, newY);
  LineTo(hDC, toFloatDraw(newX, scaleX), toFloatDraw(newY, scaleY));
}

void ContourDrawer::drawPoints(HDC& hDC, double scaleX, double scaleY, Contour& contour)
{
  size_t numPoints = contour.size();

  if (numPoints < 1)
    return;

  std::vector<Point>& points = contour.getPoints();

  const double offset = 0.0;
  const int offsetEnd = 5;
  double x, y;

  DoublePoint centerMass = massCenter(contour);
    
  for (size_t j = 0; j < numPoints; j++)
  {
    calcNewCoords(centerMass, points[j], offset, x, y);
      
    x = toFloatDraw(x, scaleX);
    y = toFloatDraw(y, scaleY);

    Ellipse(hDC, x - offsetEnd, y - offsetEnd, x + offsetEnd, y + offsetEnd);
  }
}


HPEN ContourDrawer::getCachedPen(COLORREF color)
{
  auto iter = cachedPens.find(color);
  if (iter != cachedPens.end())
  {
    return iter->second;
  }

  HPEN newPen = CreatePen(PS_SOLID, PEN_WIDTH, color);
  cachedPens[color] = newPen;

  return newPen;
}

HBRUSH ContourDrawer::getCachedBrush(COLORREF color)
{
  auto iter = cachedBrushes.find(color);
  if (iter != cachedBrushes.end())
  {
    return iter->second;
  }

  HBRUSH newBrush = CreateSolidBrush(color);
  cachedBrushes[color] = newBrush;

  return newBrush;
}



void ContourDrawer::draw(HDC& hDC, double scaleX, double scaleY)
{
  HGDIOBJ oldPen = SelectObject(hDC, visiblePen);
  HGDIOBJ oldBrush = SelectObject(hDC, visibleBrush);

  draw(hDC, scaleX, scaleY, VISIBLE);
  draw(hDC, scaleX, scaleY, SELECTED);

  SelectObject(hDC, oldPen);
  SelectObject(hDC, oldBrush);
}

void ContourDrawer::draw(HDC& hDC, double scaleX, double scaleY, ContourState needState)
{
  std::list<Contour>& contours = dataManager.getContours();

  for (auto iter = contours.begin(); iter != contours.end(); ++iter)
  {
    ContourState state = dataManager.getContourState(*iter);
    if (state != needState)
      continue;

    COLORREF color = dataManager.getContourColor(*iter);

    SelectObject(hDC, getCachedPen(color));
    SelectObject(hDC, getCachedBrush(color));

    drawPoints(hDC, scaleX, scaleY, *iter);
    drawLines(hDC, scaleX, scaleY, *iter);
  }
}

