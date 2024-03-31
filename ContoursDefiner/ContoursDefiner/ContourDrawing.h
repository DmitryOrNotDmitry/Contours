#pragma once

#include "ERInterface.h"
#include "Contour.h"
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

  void drawControlPoints(HDC hDC);

public:
  ContourDrawing(HIMAGE hImage, const DialogListContours& contoursDlg);
  virtual ~ContourDrawing();

  void OnDraw(HDC hDC);
  void ReleaseContext() override;

};

