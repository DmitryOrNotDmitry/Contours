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
  if (hImage)
	  Detach();
}

void ContourDrawing::OnDraw(HDC hDC)
{
  COLORREF color = RGB(0, 250, 0);
  HGDIOBJ oldPen = SelectObject(hDC, CreatePen(PS_SOLID, 1, color));

  POINT* points = (POINT*)contour.getData();
  size_t numPoints = contour.size();
  Polygon(hDC, points, numPoints);
  
  SelectObject(hDC, oldPen);
}

void ContourDrawing::ReleaseContext()
{
  delete this;
}
