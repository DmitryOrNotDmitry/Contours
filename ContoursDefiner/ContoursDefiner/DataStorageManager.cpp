#include "DataStorageManager.h"

DataStorageManager DataStorageManager::instance = DataStorageManager();

DataStorageManager& DataStorageManager::getInstance()
{
  return instance;
}

DataStorageManager::DataStorageManager()
{
  showHoles = true;
}

void DataStorageManager::addContour(Contour& contour)
{
  contours.push_back(std::move(contour));
}

int DataStorageManager::getCountContours()
{
  return contours.size();
}

std::list<Contour>& DataStorageManager::getContours()
{
  return contours;
}


void DataStorageManager::addBorder(const LineBorder& border)
{
  borders.push_back(border);
}


std::vector<LineBorder>& DataStorageManager::getBorders()
{
  return borders;
}


bool DataStorageManager::isShowHoles()
{
    return showHoles;
}

bool DataStorageManager::setShowHoles(bool showHoles)
{
  return this->showHoles = showHoles;
}

void DataStorageManager::addHole(Contour&& hole)
{
  holes.push_back(hole);
}

void DataStorageManager::addHole(const Contour& hole)
{
  holes.push_back(hole);
}

std::vector<Contour>& DataStorageManager::getHoles()
{
  return holes;
}
