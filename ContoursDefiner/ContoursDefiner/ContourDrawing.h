#pragma once

#include "ERInterface.h"
#include "Contour.h"


class ContourDrawing : public CVDirectDrawing
{
  std::vector<Contour> contours;
  HIMAGE hImage;

public:
  ContourDrawing(HIMAGE hImage);
  virtual ~ContourDrawing();

  void OnDraw(HDC hDC);
  void ReleaseContext() override;

  void addContour(Contour& contour);

};

