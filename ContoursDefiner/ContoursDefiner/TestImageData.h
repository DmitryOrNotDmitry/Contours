#pragma once
#include "ImageDataManager.h"
#include <vector>

class TestImageData : public ImageDataManager
{

  std::vector<std::vector<int>> imageLines;
  int kolLines;
  int kolPix;

public:
  TestImageData();
  virtual ~TestImageData();

  int getPointValue(const Point& point, int offset = 0) override;

  int lineSize() override;
  int getCountLines() override;

  void addLine(const std::vector<int>& line);
};

