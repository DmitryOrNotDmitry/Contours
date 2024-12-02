#pragma once
#include "AbstractImageData.h"
#include "..\ERInterface.h"

#include <cstdint>

class ERImageData : public AbstractImageData
{

  HIMAGE hImage;

  uint32_t kolLines;
  uint32_t kolPix;

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

