#pragma once
#include "AbstractDrawer.h"

class BorderDrawer : public AbstractDrawer
{

  const HPEN pen;

public:

  BorderDrawer(DataStorageManager& dataManager)
    : AbstractDrawer(dataManager)
    , pen( CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 0, 0)) )
  {
  }

  ~BorderDrawer()
  {
    DeleteObject(pen);
  }

  void draw(HDC& hDC, double scaleX, double scaleY) override;

};

