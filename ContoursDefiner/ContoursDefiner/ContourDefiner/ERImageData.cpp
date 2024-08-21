#include "ERImageData.h"

ERImageData::ERImageData(HIMAGE hImage)
{
  if (hImage)
  {
    CImageInterface image(hImage);

    initData(image);
  }
  else
  {
    kolLines = 0;
    kolPix = 0;
    QChans = 0;
  }
}

ERImageData::ERImageData(CImageInterface& image)
{
  initData(image);
}

ERImageData::~ERImageData()
{
  if (kolLines > 0)
  {
    for (DWORD i = 0; i < kolLines; i++)
    {
      if (imageLines[i])
        delete[] imageLines[i];
    }
    delete[] imageLines;
  }
}

bool ERImageData::isContourPoint(const Point& point)
{
  return imageLines[point.y][point.x * QChans] == 0;
}

int ERImageData::lineSize()
{
  return kolPix;
}

int ERImageData::getCountLines()
{
  return kolLines;
}

void ERImageData::initData(CImageInterface& image)
{
  kolLines = *image.dwImageKolLines;
  kolPix = *image.dwImageKolPix;
  QChans = *image.dwQChans;

  imageLines = new unsigned char* [kolLines];
  for (DWORD i = 0; i < kolLines; i++)
  {
    imageLines[i] = new unsigned char[kolPix * QChans];
    image.ReadDataStream(imageLines[i], i, FORMAT_8);
  }
}
