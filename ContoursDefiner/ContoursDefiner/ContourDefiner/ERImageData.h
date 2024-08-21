#pragma once
#include "AbstractImageData.h"
#include "..\ERInterface.h"

class ERImageData : public AbstractImageData
{

  HIMAGE hImage;

  unsigned char** imageLines;
  DWORD kolLines;
  DWORD kolPix;
  DWORD QChans;

public:

  ERImageData(HIMAGE hImage);
  ERImageData(CImageInterface& image);
  virtual ~ERImageData();

  bool isContourPoint(const Point& point) override;

  int lineSize() override;
  int getCountLines() override;

private:
  void initData(CImageInterface& image);
};

