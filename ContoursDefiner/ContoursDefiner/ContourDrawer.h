#pragma once
#include "AbstractDrawer.h"

class ContourDrawer : public AbstractDrawer
{

  const HPEN visiblePen;
  const HPEN selectedPen;

  const HBRUSH visibleBrush;
  const HBRUSH selectedBrush;

  void drawLines(HDC& hDC, double scaleX, double scaleY, ContourState state);
  void drawPoints(HDC& hDC, double scaleX, double scaleY, ContourState state);

public:

  ContourDrawer(DataStorageManager& dataManager)
    : AbstractDrawer(dataManager)
    , visiblePen( CreatePen(PS_SOLID, PEN_WIDTH, RGB(0, 250, 250)) )
    , selectedPen( CreatePen(PS_SOLID, PEN_WIDTH, RGB(0, 250, 0)) )
    , visibleBrush( CreateSolidBrush(RGB(0, 250, 250)) )
    , selectedBrush( CreateSolidBrush(RGB(0, 250, 0)) )
  {
  }

  ~ContourDrawer()
  {
    DeleteObject(visiblePen);
    DeleteObject(selectedPen);
  }

  void draw(HDC& hDC, double scaleX, double scaleY) override;

};

