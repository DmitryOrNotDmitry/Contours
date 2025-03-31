#include "DataStorageManager.h"


DataStorageManager DataStorageManager::instance = DataStorageManager();

DataStorageManager& DataStorageManager::getInstance()
{
  return instance;
}

DataStorageManager::DataStorageManager()
{
  showHoles = true;
  colorsPool = ColorsPool::getInstance();
}

void DataStorageManager::addContour(Contour& contour)
{
  contours.push_back(std::move(contour));

  Contour& lastContour = contours.back();
  contoursStates.insert(std::make_pair(&lastContour, ContourTraits()));
  contoursStates[&lastContour].colorId = colorsPool->getNextColorId();
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

COLORREF DataStorageManager::getHoleOwnerColor(Contour& hole)
{
  auto iter = holeOwner.find(&hole);
  if (iter == holeOwner.end()) {
    return RGB(0, 255, 0);
  }

  return getContourColor(*iter->second);
}

void DataStorageManager::setHoleOwner(Contour& hole, Contour& owner)
{
  holeOwner.insert(std::make_pair(&hole, &owner));
}

void DataStorageManager::setContourState(Contour& c, ContourState state)
{
  contoursStates[&c].state = state;
}

ContourState DataStorageManager::getContourState(Contour& c)
{
  return contoursStates[&c].state;
}

COLORREF DataStorageManager::getContourColor(Contour& c)
{
  return colorsPool->getColorBy(contoursStates[&c].colorId);
}

void DataStorageManager::clearState(Contour& c)
{
  contoursStates.erase(&c);
}
