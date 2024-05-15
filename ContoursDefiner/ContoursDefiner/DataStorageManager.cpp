#include "DataStorageManager.h"

DataStorageManager DataStorageManager::instance = DataStorageManager();

DataStorageManager& DataStorageManager::getInstance()
{
  return instance;
}

DataStorageManager::DataStorageManager()
{
  // TODO: IT WILL NOT WORK IF NUMBER OF CONTOURS IS MORE THAN 100
  contours.reserve(100);
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


void DataStorageManager::addBorder(const LineBorder& border)
{
  borders.push_back(border);
}


std::vector<LineBorder>& DataStorageManager::getBorders()
{
  return borders;
}


void DataStorageManager::addAverageBorder(std::vector<Point>&& averageBorder)
{
  averageBorders.push_back(averageBorder);
}

std::vector<std::vector<Point>>& DataStorageManager::getAverageBorders()
{
  return averageBorders;
}