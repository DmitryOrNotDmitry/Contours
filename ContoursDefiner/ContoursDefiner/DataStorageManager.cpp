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

std::vector<Contour*> DataStorageManager::getPContours()
{
  std::vector<Contour*> pContours;
  for (auto iterCont = contours.begin(); iterCont != contours.end(); ++iterCont)
    pContours.push_back(&(*iterCont));
  return pContours;
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

void DataStorageManager::setContourState(Contour& c, ContourState state)
{
  contoursStates[&c] = state;
}

ContourState DataStorageManager::getContourState(Contour& c)
{
  return contoursStates[&c];
}

void DataStorageManager::clearState(Contour& c)
{
  contoursStates.erase(&c);
}
