#include "DataStorageManager.h"

DataStorageManager DataStorageManager::instance = DataStorageManager();

DataStorageManager& DataStorageManager::getInstance()
{
  return instance;
}

DataStorageManager::DataStorageManager()
{

}

void DataStorageManager::addContour(Contour& contour)
{
  contours.push_back(std::move(contour));
}

int DataStorageManager::getCountContours()
{
  return contours.size();
}

std::vector<Contour>& DataStorageManager::getContours()
{
  return contours;
}

void DataStorageManager::addControlPoint(const Point& point)
{
  controlPoints.push_back(point);
}

std::vector<Point>& DataStorageManager::getControlPoints() 
{
  return controlPoints;
}
