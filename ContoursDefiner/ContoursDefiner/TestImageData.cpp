#include "TestImageData.h"

TestImageData::TestImageData()
{
  kolLines = 0;
  kolPix = 0;
}

TestImageData::~TestImageData()
{
}

int TestImageData::getPointValue(const Point& point, int offset)
{
  return imageLines[point.y][point.x];
}

int TestImageData::lineSize()
{
  return kolPix;
}

int TestImageData::getCountLines()
{
  return kolLines;
}

void TestImageData::addLine(const std::vector<int>& line)
{
  imageLines.push_back(line);
  kolLines++;
  kolPix = line.size();
}
