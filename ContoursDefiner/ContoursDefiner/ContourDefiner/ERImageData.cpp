#include "ERImageData.h"

ERImageData::ERImageData(HIMAGE hImage)
  : hImage(hImage)
{
  initData();
}

ERImageData::ERImageData(CImageInterface& image)
  : hImage(image.hAttachImage)
{
  initData();
}

ERImageData::~ERImageData()
{
}

bool ERImageData::isContourPoint(const Point& point)
{
  BYTE buf[3];
  ReadDataStream(hImage, buf, point.y, FORMAT_8, point.x, 1);

  return buf[0] == 0 && buf[1] == 0 && buf[2] == 0;
}

int ERImageData::lineSize()
{
  return kolPix;
}

int ERImageData::getCountLines()
{
  return kolLines;
}

void ERImageData::initData()
{
  if (hImage)
  {
    kolLines = DI_dwImageKolLines(hImage);
    kolPix = DI_dwImageKolPix(hImage);
  }
  else
  {
    kolLines = 0;
    kolPix = 0;
  }
}
