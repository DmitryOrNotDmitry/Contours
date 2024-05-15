#pragma once
#include "AbstractDrawer.h"

class HoleDrawer : public AbstractDrawer
{

  const HPEN pen;

public:

  HoleDrawer(DataStorageManager& dataManager)
    : AbstractDrawer(dataManager)
    , pen( CreatePen(PS_SOLID, PEN_WIDTH, RGB(0, 0, 255)) )
  {
  }

  ~HoleDrawer()
  {
    DeleteObject(pen);
  }

  void draw(HDC& hDC, double scaleX, double scaleY) override;

};

