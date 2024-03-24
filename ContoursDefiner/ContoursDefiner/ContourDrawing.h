#pragma once

#include "ERInterface.h"
#include "Contour.h"
#include "FirstPointSetting.h"
#include "ContourState.h"


struct ContourView
{
  ContourView(Contour contour, ContourState state = ContourState::VISIBLE);

  Contour contour;
  ContourState state;
};


class ContourDrawing : public CVDirectDrawing
{
  std::vector<ContourView> contours;
  HIMAGE hImage;
  const FirstPointSetting& contoursDlg;

public:
  ContourDrawing(HIMAGE hImage, const FirstPointSetting& contoursDlg);
  virtual ~ContourDrawing();

  void OnDraw(HDC hDC);
  void ReleaseContext() override;

  void addContour(Contour& contour);
  int getCountContours();

};

