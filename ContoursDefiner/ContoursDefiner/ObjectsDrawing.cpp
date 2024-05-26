#include "StdAfx.h"
#include "ObjectsDrawing.h"
#include "ContourDrawer.h"
#include "BorderDrawer.h"
#include "HoleDrawer.h"
#include <memory>

ObjectsDrawing::ObjectsDrawing(HIMAGE hImage, const DialogListContours& contoursDlg)
  : hImage(hImage)
  , contoursDlg(contoursDlg)
  , dataManager(DataStorageManager::getInstance())
{
  drawers.push_back( std::unique_ptr<AbstractDrawer>(new ContourDrawer(dataManager)) );
  drawers.push_back( std::unique_ptr<AbstractDrawer>(new HoleDrawer(dataManager)) );
  drawers.push_back( std::unique_ptr<AbstractDrawer>(new BorderDrawer(dataManager)) );
  
  Attach(this->hImage);
  Update();
  RecalcImageViews(this->hImage);
}

ObjectsDrawing::~ObjectsDrawing()
{
  if (hImage)
  {
	  Detach();
    hImage = nullptr;
  }
}


void ObjectsDrawing::OnFLoatDraw(HDC hDC, double scaleX, double scaleY)
{
  for (size_t i = 0; i < drawers.size(); i++)
  {
    drawers[i]->draw(hDC, scaleX, scaleY);
  }
}


void ObjectsDrawing::ReleaseContext()
{
  delete this;
}
