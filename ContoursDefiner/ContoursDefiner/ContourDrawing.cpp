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


int toFloatDraw(int coordinate, double scale)
{
  return (int)((coordinate + 0.5) * scale);
}


void ContourDrawing::drawControlPoints(HDC& hDC, double scaleX, double scaleY)
{
  HPEN controlPointPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
  HGDIOBJ oldPen = SelectObject(hDC, controlPointPen);
  
  for (size_t i = 0; i < controlPoints.size(); i++)
  {
    int x = toFloatDraw(controlPoints[i].x, scaleX);
    int y = toFloatDraw(controlPoints[i].y, scaleY);
    MoveToEx(hDC, x, y, NULL);
    LineTo(hDC, x, y);
  }
  
  DeleteObject(controlPointPen);
  
  SelectObject(hDC, oldPen);
}

void ContourDrawing::drawBorders(HDC& hDC, double scaleX, double scaleY)
{
  HPEN bordersPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
  HGDIOBJ oldPen = SelectObject(hDC, bordersPen);

  for (size_t i = 0; i < borders.size(); i++)
  {
    if (borders[i].getOwner().getState() == HIDDEN)
      continue;

    const std::vector<Point>& points = borders[i].getOwner().getPoints();
    int numPoints = (int) points.size();

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

  DeleteObject(bordersPen);

  SelectObject(hDC, oldPen);
}

void ContourDrawing::drawAverageBorders(HDC& hDC, double scaleX, double scaleY)
{
  HPEN bordersPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
  HGDIOBJ oldPen = SelectObject(hDC, bordersPen);

  int count_borders = dataManager.getAverageBorders().size();
  for (size_t i = 0; i < count_borders; i++)
  {
    const std::vector<Point>& points = dataManager.getAverageBorders()[i];
    int numPoints = (int)points.size();

    MoveToEx(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY), NULL);
    for (int j = 0; j < numPoints; j++)
    {
      LineTo(hDC, toFloatDraw(points[j].x, scaleX), toFloatDraw(points[j].y, scaleY));
    }
  }

  DeleteObject(bordersPen);

  SelectObject(hDC, oldPen);
}


void ContourDrawing::drawContoursWithState(HDC& hDC, double scaleX, double scaleY, ContourState state)
{
  for (size_t i = 0; i < contours.size(); i++)
  {
    if (contours[i].getState() != state)
      continue;

    size_t numPoints = contours[i].size();

    if (numPoints < 1)
      continue;

    std::vector<Point>& points = contours[i].getPoints();;

    MoveToEx(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY), NULL);
    for (size_t j = 1; j < numPoints; j++)
    {
      LineTo(hDC, toFloatDraw(points[j].x, scaleX), toFloatDraw(points[j].y, scaleY));
    }
    LineTo(hDC, toFloatDraw(points[0].x, scaleX), toFloatDraw(points[0].y, scaleY));
  }
}


void ContourDrawing::OnFLoatDraw(HDC hDC, double scaleX, double scaleY)
{
  COLORREF visibleColor = RGB(0, 250, 250);
  COLORREF selectedColor = RGB(0, 250, 0);
  int penWidth = 5;
  HPEN visiblePen = CreatePen(PS_SOLID, penWidth, visibleColor);
  HPEN selectedPen = CreatePen(PS_SOLID, penWidth, selectedColor);
  

  HGDIOBJ oldPen = SelectObject(hDC, visiblePen);
  drawContoursWithState(hDC, scaleX, scaleY, VISIBLE);

  SelectObject(hDC, selectedPen);
  drawContoursWithState(hDC, scaleX, scaleY, SELECTED);
  

  DeleteObject(visiblePen);
  DeleteObject(selectedPen);

  drawControlPoints(hDC, scaleX, scaleY);
  
  drawBorders(hDC, scaleX, scaleY);

  //drawAverageBorders(hDC, scaleX, scaleY);
 
  SelectObject(hDC, oldPen);
}


void ContourDrawing::ReleaseContext()
{
  delete this;
}
