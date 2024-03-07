#pragma once
#include "ImageDataManager.h"
#include "ERInterface.h"

class ERImageData : public ImageDataManager
{

  HIMAGE hImage;

  unsigned char** imageLines;
  DWORD kolLines;
  DWORD kolPix;
  DWORD QChans;

public:

  ERImageData(HIMAGE hImage);
  //ERImageData(const ERImageData& other);
  virtual ~ERImageData();

  int getPointValue(const Point& point, int offset = 0) override;

  int lineSize() override;
  int getCountLines() override;
};

