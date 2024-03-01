#include "StdAfx.h"
#include "ContourDrawing.h"


ContourDrawing::ContourDrawing(HIMAGE hImage, Contour& contour)
  : contour(contour)
  , hImage(hImage)
{
  Attach(this->hImage);
  Update();
  RecalcImageViews(this->hImage);
}

ContourDrawing::~ContourDrawing()
{
  Detach();
}

void ContourDrawing::OnDraw(HDC hDC)
{
  HGDIOBJ oldPen = SelectObject(hDC, CreatePen(PS_SOLID, 1, RGB(0, 255, 0)));
  
  POINT* points = (POINT*)contour.getData();
  size_t numPoints = contour.size();
  Polygon(hDC, points, numPoints);
  
  SelectObject(hDC, oldPen);
}