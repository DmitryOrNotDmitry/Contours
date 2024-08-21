#pragma once
#include "AbstractImageData.h"
#include "..\ERInterface.h"

class ERImageData : public AbstractImageData
{

  HIMAGE hImage;

  DWORD kolLines;
  DWORD kolPix;

public:

  ERImageData(HIMAGE hImage);
  ERImageData(CImageInterface& image);
  virtual ~ERImageData();

  bool isContourPoint(const Point& point) override;

  int lineSize() override;
  int getCountLines() override;

private:
  void initData();
};

