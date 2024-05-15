#pragma once

#include "ERInterface.h"
#include "Contour.h"
#include "LineBorder.h"
#include "DialogListContours.h"
#include "ContourState.h"
#include "DataStorageManager.h"


class ContourDrawing : public CVDirectDrawing
{
  HIMAGE hImage;
  const DialogListContours& contoursDlg;

  DataStorageManager& dataManager;

  std::vector<Contour>& contours;
  std::vector<Point>& controlPoints;
  std::vector<LineBorder>& borders;

  void drawControlPoints(HDC& hDC, double scaleX, double scaleY);
  
  void drawBorders(HDC& hDC, double scaleX, double scaleY);
  
  void drawAverageBorders(HDC& hDC, double scaleX, double scaleY);
  
  void drawContoursWithState(HDC& hDC, double scaleX, double scaleY, ContourState state = VISIBLE);

public:
  ContourDrawing(HIMAGE hImage, const DialogListContours& contoursDlg);
  virtual ~ContourDrawing();

  void OnFLoatDraw(HDC hDC, double scaleX, double scaleY) override;
  void ReleaseContext() override;

};

