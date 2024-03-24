#include "StdAfx.h"
#include "ContourDrawing.h"

ContourDrawing::ContourDrawing(HIMAGE hImage, const FirstPointSetting& contoursDlg)
  : hImage(hImage)
  , contoursDlg(contoursDlg)
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
  COLORREF visibleColor = RGB(0, 250, 0);
  COLORREF selectedColor = RGB(0, 0, 250);
  HGDIOBJ oldPen = SelectObject(hDC, CreatePen(PS_SOLID, 1, visibleColor));
  
  if (contoursDlg)
  {
    std::vector<ContourState> states = contoursDlg.getContoursStates();
    for (size_t i = 0; i < states.size(); i++)
    {
      contours[i].state = states[i];
    }
  }

  for (size_t i = 0; i < contours.size(); i++)
  {
    if (contours[i].state == ContourState::HIDDEN)
      continue;

    if (contours[i].state == ContourState::VISIBLE)
      SelectObject(hDC, CreatePen(PS_SOLID, 1, visibleColor));

    if (contours[i].state == ContourState::SELECTED)
      SelectObject(hDC, CreatePen(PS_SOLID, 1, selectedColor));


    Point* points = contours[i].contour.getData();
    size_t numPoints = contours[i].contour.size();
    
    if (numPoints < 1)
      continue;

    MoveToEx(hDC, points[0].x, points[0].y, NULL);
    for (size_t j = 1; j < numPoints; j++)
    {
      LineTo(hDC, points[j].x, points[j].y);
    }
    LineTo(hDC, points[0].x, points[0].y);
  }
  
  SelectObject(hDC, oldPen);
}


void ContourDrawing::ReleaseContext()
{
  delete this;
}


void ContourDrawing::addContour(Contour& contour)
{
  contours.push_back(ContourView(std::move(contour)));
  RecalcImageViews(hImage);
}

int ContourDrawing::getCountContours()
{
    return contours.size();
}

ContourView::ContourView(Contour contour, ContourState state)
  : contour(contour)
  , state(state)
{
}
