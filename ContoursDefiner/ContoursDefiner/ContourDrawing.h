#pragma once

#include "ERInterface.h"
#include "Contour.h"


struct ContourView
{
  ContourView::ContourView(Contour contour, bool isVisible = true);

  Contour contour;
  bool isVisible;
};


class ContourDrawing : public CVDirectDrawing
{
  std::vector<ContourView> contours;
  HIMAGE hImage;

public:
  ContourDrawing(HIMAGE hImage);
  virtual ~ContourDrawing();

  void OnDraw(HDC hDC);
  void ReleaseContext() override;

  void addContour(Contour& contour);
  int getCountContours();

};

