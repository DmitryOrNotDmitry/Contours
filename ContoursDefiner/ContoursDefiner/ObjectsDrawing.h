#pragma once

#include "ERInterface.h"
#include "ContourDefiner/Contour.h"
#include "ContourDefiner/LineBorder.h"
#include "DialogListContours.h"
#include "ContourState.h"
#include "DataStorageManager.h"
#include "AbstractDrawer.h"


class ObjectsDrawing : public CVDirectDrawing
{
  HIMAGE hImage;
  const DialogListContours& contoursDlg;

  DataStorageManager& dataManager;

  std::vector<std::unique_ptr<AbstractDrawer>> drawers;

public:
  ObjectsDrawing(HIMAGE hImage, const DialogListContours& contoursDlg);
  virtual ~ObjectsDrawing();

  void OnFLoatDraw(HDC hDC, double scaleX, double scaleY) override;
  void ReleaseContext() override;

};

