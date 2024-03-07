#pragma once

#include "ERInterface.h"
#include "Contour.h"


class ContourDrawing : public CVDirectDrawing
{
  Contour& contour;
  HIMAGE hImage;

public:
  ContourDrawing(HIMAGE hImage, Contour& contour);
  virtual ~ContourDrawing();

  void OnDraw(HDC hDC);
  void ReleaseContext() override;

};

