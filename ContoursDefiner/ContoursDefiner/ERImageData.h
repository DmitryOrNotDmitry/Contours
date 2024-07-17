#pragma once
#include "AbstractImageData.h"
#include "ERInterface.h"

class ERImageData : public AbstractImageData
{

  HIMAGE hImage;

  unsigned char** imageLines;
  DWORD kolLines;
  DWORD kolPix;
  DWORD QChans;

public:

  ERImageData(HIMAGE hImage);
  virtual ~ERImageData();

  int getPointValue(const Point& point, int offset = 0) override;

  int lineSize() override;
  int getCountLines() override;
};

