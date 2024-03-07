#include "ERImageData.h"

ERImageData::ERImageData(HIMAGE hImage)
  : hImage(hImage)
{
  if (hImage)
  {
    CImageInterface image(hImage);

    kolLines = *image.dwImageKolLines;
    kolPix = *image.dwImageKolPix;
    QChans = *image.dwQChans;

    imageLines = new unsigned char* [kolLines];
    for (int i = 0; i < kolLines; i++)
    {
      imageLines[i] = new unsigned char[kolPix * QChans];
      image.ReadDataStream(imageLines[i], i, FORMAT_8);
    }
  }
  else
  {
    kolLines = 0;
    kolPix = 0;
    QChans = 0;
  }
}

//ERImageData::ERImageData(const ERImageData& other)
//  : hImage(other.hImage)
//  , kolLines(other.kolLines)
//  , kolPix(other.kolPix)
//  , QChans(other.QChans)
//{
//  imageLines = new unsigned char* [kolLines];
//  for (int i = 0; i < kolLines; i++)
//  {
//    imageLines[i] = new unsigned char[kolPix * QChans];
//    for (int j = 0; j < kolLines; j++)
//      imageLines[i][j] = other.imageLines[i][j];
//  }
//}

ERImageData::~ERImageData()
{
  if (kolLines > 0)
  {
    for (int i = 0; i < kolPix; i++)
    {
      if (imageLines[i])
        delete[] imageLines[i];
    }
    delete[] imageLines;
  }
}

int ERImageData::getPointValue(const Point& point, int offset)
{
  return imageLines[point.y][point.x * QChans + offset];
}

int ERImageData::lineSize()
{
  return kolPix;
}

int ERImageData::getCountLines()
{
  return kolLines;
}
