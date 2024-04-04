#include "StdAfx.h"
#include "ContourDrawing.h"

ContourDrawing::ContourDrawing(HIMAGE hImage, const DialogListContours& contoursDlg)
  : hImage(hImage)
  , contoursDlg(contoursDlg)
  , dataManager(DataStorageManager::getInstance())
  , contours(dataManager.getContours())
  , controlPoints(dataManager.getControlPoints())
  , borders(dataManager.getBorders())
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

void ContourDrawing::drawControlPoints(HDC& hDC)
{
  HPEN controlPointPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  HGDIOBJ oldPen = SelectObject(hDC, controlPointPen);
  
  for (size_t i = 0; i < controlPoints.size(); i++)
  {
    MoveToEx(hDC, controlPoints[i].x, controlPoints[i].y, NULL);
    LineTo(hDC, controlPoints[i].x, controlPoints[i].y);
  }
  
  DeleteObject(controlPointPen);
  
  SelectObject(hDC, oldPen);
}

void ContourDrawing::drawBorders(HDC& hDC)
{
  HPEN bordersPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  HGDIOBJ oldPen = SelectObject(hDC, bordersPen);

  for (size_t i = 0; i < borders.size(); i++)
  {
    const std::vector<Point>& points = borders[i].getOwner()->getPoints();
    int numPoints = (int) points.size();

    int from = borders[i].getFromIndex();
    int to = borders[i].getToIndex();

    MoveToEx(hDC, points[from].x, points[from].y, NULL);
    for (int j = from; j != (to + 1); j++)
    {
      if (j >= numPoints)
        j = 0;

      LineTo(hDC, points[j].x, points[j].y);
    }
  }

  DeleteObject(bordersPen);

  SelectObject(hDC, oldPen);
}

void ContourDrawing::OnDraw(HDC hDC)
{
  COLORREF visibleColor = RGB(0, 250, 250);
  COLORREF selectedColor = RGB(0, 250, 0);
  HPEN visiblePen = CreatePen(PS_SOLID, 1, visibleColor);
  HPEN selectedPen = CreatePen(PS_SOLID, 1, selectedColor);
  HGDIOBJ oldPen = SelectObject(hDC, visiblePen);

  for (size_t i = 0; i < contours.size(); i++)
  {
    if (contours[i].getState() == HIDDEN)
      continue;

    if (contours[i].getState() == VISIBLE)
      SelectObject(hDC, visiblePen);

    if (contours[i].getState() == SELECTED)
      SelectObject(hDC, selectedPen);


    Point* points = contours[i].getData();
    size_t numPoints = contours[i].size();
    
    if (numPoints < 1)
      continue;

    MoveToEx(hDC, points[0].x, points[0].y, NULL);
    for (size_t j = 1; j < numPoints; j++)
    {
      LineTo(hDC, points[j].x, points[j].y);
    }
    LineTo(hDC, points[0].x, points[0].y);
  
  }

  DeleteObject(visiblePen);
  DeleteObject(selectedPen);

  drawControlPoints(hDC);
  
  drawBorders(hDC);
 
  SelectObject(hDC, oldPen);
}


void ContourDrawing::ReleaseContext()
{
  delete this;
}
