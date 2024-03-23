#include "StdAfx.h"
#include "ContourDrawing.h"

ContourDrawing::ContourDrawing(HIMAGE hImage)
  : hImage(hImage)
{
  Attach(this->hImage);
  Update();
  RecalcImageViews(this->hImage);
}

ContourDrawing::~ContourDrawing()
{
  if (hImage)
  {
	  Detach();
    hImage = nullptr;
  }
}

void ContourDrawing::OnDraw(HDC hDC)
{
  COLORREF color = RGB(0, 250, 0);
  HGDIOBJ oldPen = SelectObject(hDC, CreatePen(PS_SOLID, 1, color));

  for (size_t i = 0; i < contours.size(); i++)
  {
    POINT* points = (POINT*)contours[i].getData();
    size_t numPoints = contours[i].size();
    Polygon(hDC, points, numPoints);
  }
  
  SelectObject(hDC, oldPen);
}

void ContourDrawing::ReleaseContext()
{
  delete this;
}

void ContourDrawing::addContour(Contour& contour)
{
  contours.push_back(std::move(contour));
  RecalcImageViews(hImage);
}
