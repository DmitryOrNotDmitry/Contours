#pragma once
#include "stdafx.h"
#include "DataStorageManager.h"

class AbstractDrawer
{

protected:
  
  const static int PEN_WIDTH = 5;

  DataStorageManager& dataManager;

  AbstractDrawer(DataStorageManager& dataManager) 
    : dataManager(dataManager)
  {
  }

  int toFloatDraw(int coordinate, double scale)
  {
    return (int)((coordinate + 0.5) * scale);
  }

  int toFloatDraw(double coordinate, double scale)
  {
    return (int)((coordinate + 0.5) * scale);
  }

  DoublePoint massCenter(Contour& contour);
  void calcNewCoords(DoublePoint massCenter, Point point, double offset, double& x, double& y);

public:

  virtual void draw(HDC& hDC, double scaleX, double scaleY) = 0;

};

