#include "Contour.h"
#include "LineSmoother.h"
#include "LineBorder.h"
#include "GeneralBorderCalculator.h"
#include "LineBorderVector.h"

#include <unordered_set>
#include <algorithm>
#include <stack>
#include <set>



namespace std {
  template <>
  struct std::hash<Point> {
    size_t operator()(const Point& point) const {
      return hash<int>()(point.x) ^ (hash<int>()(point.y) << 1);
    }
  };
}


Contour::Contour()
{
}

Contour::Contour(const Contour& other)
  : points(other.points)
{
}


Contour::~Contour()
{
  for (size_t i = 0; i < innerContours.size(); i++)
    delete innerContours[i];
}

Contour& Contour::operator=(const Contour& other)
{
  points = other.points;

  return *this;
}

void Contour::addInnerContour(Contour* innerContour)
{
  innerContours.push_back(innerContour);
}

Contour* Contour::getInnerContour(size_t idx)
{
  if (idx >= 0 && idx < innerContours.size())
    return innerContours[idx];

  return nullptr;
}

void Contour::removeInnerContour(size_t idx)
{
  if (idx >= 0 && idx < innerContours.size())
  {
    Contour* deletedCont = innerContours[idx];
    delete deletedCont;

    innerContours.erase(innerContours.begin() + idx);
  }
   
}

size_t Contour::countInnerContours()
{
  return innerContours.size();
}

std::vector<Contour*> Contour::getAllInnerContour()
{
  return innerContours;
}


void Contour::addPoint(Point point)
{
  if (points.size() > 0)
  {
    if (*points.rbegin() == point)
      return;
  }
  
  points.push_back(point);
}

void Contour::insertPoint(Point point, int index)
{
  int size = static_cast<int>(this->size());

  if (index > 0 && (*(points.begin() + index - 1) == point))
    return;

  if (index < size && (*(points.begin() + index) == point))
    return;

  if (index == size && size > 1 && (points[0] == point))
    return;

  points.insert(points.begin() + index, point);
}


std::vector<Point>& Contour::getPoints()
{
  return points;
}


size_t Contour::size() const
{
  return points.size();
}


bool Contour::operator==(const Contour& other) const
{
  bool result = true;

  std::unordered_set<Point> ourPoints(points.begin(), points.end());

  for (auto iter = other.points.begin(); iter != other.points.end(); ++iter)
  {
    if (ourPoints.find(*iter) == ourPoints.end())
    {
      result = false;
      break;
    }
  }

  return result;
}

bool Contour::operator!=(const Contour& other) const
{
  return !(*this == other);
}

Point& Contour::operator[](int i)
{
  return points[i];
}

const Point& Contour::operator[](int i) const
{
  return points[i];
}

bool Contour::isEmpty() const
{
  return points.empty();
}

int Contour::findNearestPointTo(const Point& destination, int from, int count, int step) const
{
  double minDist = 10e100;
  int minItemIndex = 0;

  int realIndex = from;

  for (int i = 0; i < count; i++)
  {
    double curDist = points[realIndex].DistanceTo(destination);
    if (curDist < minDist)
    {
      minDist = curDist;
      minItemIndex = realIndex;
    }
    realIndex = getNextIdx(realIndex, step);
  }

  return minItemIndex;
}


int Contour::findNearestPointTo(const Point& destination, int step) const
{
  return findNearestPointTo(destination, 0, points.size(), step);
}


void Contour::removeSamePointsAtEnds()
{
  if (points.size() == 0)
    return;

  Point& lastPoint = *points.rbegin();
  
  int sameIdx = -1;
  for (int i = 0; i < 8; i++)
  {
    if (points[i] == lastPoint)
    {
      sameIdx = i;
      break;
    }
  }

  if (sameIdx == -1)
    return;

  bool isEqualsPointChain = true;
  for (int i = 0; i < sameIdx; i++)
  {
    if (*(points.rbegin() + i) != points[sameIdx - i])
    {
      isEqualsPointChain = false;
      break;
    }
  }

  if (isEqualsPointChain)
  {
    points.erase(points.end() - (sameIdx + 1), points.end());
  }

}


double Contour::signArea(int from, int to) const
{
  double area = 0;

  for (int i = from; i != to; i = getNextIdx(i, 1))
  {
    int nextI = getNextIdx(i, 1);
    area += points[i].x * points[nextI].y - points[i].y * points[nextI].x;
  }

  area += points[to].x * points[from].y - points[to].y * points[from].x;

  area /= 2;

  return area;
}


Point Contour::getPoint(int index) const
{
  return points[index];
}

int Contour::getNextIdx(int fromIndex, int step) const
{
  int size = this->size();
  int nextIndex = fromIndex + step;

  if (nextIndex < 0)
    nextIndex += size;

  if (nextIndex > size - 1)
  {
    nextIndex %= size;
  }

  return nextIndex;
}


double Contour::distanceTo(const Point& destination) const
{
  int nearestPointIdx = findNearestPointTo(destination);
  return destination.DistanceTo(getPoint(nearestPointIdx));
}


double Contour::area() const
{
  return area(0, size() - 1);
}

double Contour::area(int from, int to) const
{
  return abs(signArea(from, to));
}

bool Contour::isClockwise(int from, int to) const
{
  return signArea(from, to) < 0;
}

bool Contour::isClockwise() const
{
  return isClockwise(0, size() - 1);
}


void Contour::deletePins()
{
  int size = static_cast<int>(this->size());
  for (int i = 0; i < size; i++)
  {
    int idxSamePoint = indexOf(getPoint(i), getNextIdx(i, 1), size - 1);

    if (idxSamePoint == -1)
      continue;

    double partArea = area(i, idxSamePoint);

    if (partArea < 0.001)
    {
      int countDeleted = deletePoints(i + 1, idxSamePoint);
    }
  }
}

int Contour::indexOf(const Point& point) const
{
  return indexOf(point, 0, size());
}

int Contour::indexOf(const Point& point, int from, int count) const
{
  int idx = from;

  for (int i = 0; i < count; i++)
  {
    if (getPoint(idx) == point)
      return idx;
    
    idx = getNextIdx(idx, 1);
  }

  return -1;
}

int Contour::deletePoints(int from, int to)
{
  int countPoints = static_cast<int>(size());
  if (from > countPoints || to > countPoints)
    return 0;

  int dist = distance(from, to);

  if (from <= to)
  {
    points.erase(points.begin() + from, points.begin() + to + 1);
  }
  else
  {
    points.erase(points.begin() + from, points.end());

    points.erase(points.begin(), points.begin() + to + 1);
  }

  return dist + 1;
}

int Contour::distance(int from, int to) const
{
  int result = to - from;

  if (from > to)
    result += size();
  
  return result;
}

int Contour::minStep(int from, int to) const
{
  int d1 = distance(from, to);
  int d2 = -distance(to, from);
  
  if (std::abs(d2) < std::abs(d1))
    d1 = d2;

  return d1;
}

bool Contour::contains(const Point& point) const
{
  return std::find(points.begin(), points.end(), point) != points.end();
}

bool isLeft(Point v1, Point v2, Point p) {
  return ((v2.x - v1.x) * (p.y - v1.y) - (p.x - v1.x) * (v2.y - v1.y)) > 0;
}

bool Contour::isInner(const Point& point) const  {
  int len = size();
  int windingNumber = 0;

  for (int i = 0; i < len; i++) {
    Point v1 = getPoint(i % len);
    Point v2 = getPoint((i + 1) % len);

    if (v1.y <= point.y) {
      if (v2.y > point.y && isLeft(v1, v2, point)) {
        windingNumber++;
      }
    }
    else {
      if (v2.y <= point.y && !isLeft(v1, v2, point)) {
        windingNumber--;
      }
    }
  }

  return windingNumber != 0;
}

bool isLeft(DoublePoint v1, DoublePoint v2, DoublePoint p) {
  return ((v2.x - v1.x) * (p.y - v1.y) - (p.x - v1.x) * (v2.y - v1.y)) > 0;
}

bool Contour::isInner(const DoublePoint& point) const
{
  int len = size();
  int windingNumber = 0;

  for (int i = 0; i < len; i++) {
    DoublePoint v1 = DoublePoint( static_cast<double>(getPoint(i % len).x), static_cast<double>(getPoint(i % len).y) );
    DoublePoint v2 = DoublePoint(static_cast<double>(getPoint((i + 1) % len).x), static_cast<double>(getPoint((i + 1) % len).y) );

    if (v1.y <= point.y) {
      if (v2.y > point.y && isLeft(v1, v2, point)) {
        windingNumber++;
      }
    }
    else {
      if (v2.y <= point.y && !isLeft(v1, v2, point)) {
        windingNumber--;
      }
    }
  }

  return windingNumber != 0;
}


std::vector<Contour*> Contour::calcNeighbors(std::vector<Contour*>& contours)
{
  std::vector<Contour*> neighbors;

  std::unordered_set<Point> ourPoints(points.begin(), points.end());

  for (auto iter = contours.begin(); iter != contours.end(); ++iter)
  {
    Contour& cont = **iter;
    for (size_t i = 0; i < cont.size(); i++)
    {
      if (ourPoints.find(cont[i]) != ourPoints.end())
      {
        neighbors.push_back(&cont);
        break;
      }
    }
  }

  return neighbors;
}


std::vector<Contour> Contour::separate() const
{
  std::vector<int> scores(static_cast<int>(size()), -1);

  int curScore = 0;
  int maxScore = 0;
  int size = static_cast<int>(this->size());
  
  for (int i = 0; i < size; i++)
  {
    if (scores[i] != -1)
    {
      curScore--;
      continue;
    }

    int samePointIdx = indexOf(getPoint(i), getNextIdx(i, 1), size - 1);
    if (samePointIdx != -1)
    {
      curScore++;
      maxScore = std::max(maxScore, curScore);
      scores[samePointIdx] = curScore;
    }

    scores[i] = curScore;
  }

 if (maxScore == 0)
    return std::vector<Contour>(1, *this);

  std::vector<Contour> possibleSubContours(maxScore + 1);
  
  int prevScore = 0;
  size = static_cast<int>(this->size());

  for (int i = 0; i < size; i++)
  {
    curScore = scores[i];

    possibleSubContours[curScore].addPoint(getPoint(i));
    
    if (curScore > prevScore)
      possibleSubContours[prevScore].addPoint(getPoint(i));

    prevScore = curScore;
  }

  std::vector<Contour> subContours;

  for (size_t i = 0; i < possibleSubContours.size(); i++)
    if (possibleSubContours[i].area() > 0.001)
      subContours.push_back(std::move(possibleSubContours[i]));

  if (subContours.size() == 0)
    subContours.push_back(*this);

  return subContours;
}


void Contour::smooth(double epsilon, std::vector<Contour*>& allContours)
{
  std::vector<std::pair<Point, Point>> savedPointsThis;
  std::vector<std::pair<Point, Point>> savedPointsOthers;
  std::vector<LineBorderVector> smoothedLines;

  std::set<LineBorder> allContourBorders;
  std::vector<std::pair<Point, Point>> savedPointsAlone;
  std::vector<LineBorderVector> smoothedLinesAlone;

  std::vector<std::pair<LineBorder, LineBorder>> allBorders;
  for (auto iter = allContours.begin(); iter != allContours.end(); ++iter)
  {
    Contour* otherCont = *iter;

    if (otherCont == this)
      continue;
    
    auto bordersWithContour = GeneralBorderCalculator::defineAllGeneralBorders(*this, *otherCont, 0);
    allBorders.insert(allBorders.end(), bordersWithContour.begin(), bordersWithContour.end());
  }
  
  if (allBorders.size() == 0)
  {
    std::vector<Point> smoothedRegion;

    smoothedRegion.insert(smoothedRegion.begin(), points.begin(), points.end());

    smoothedRegion.insert(smoothedRegion.begin(), *points.rbegin());
    smoothedRegion.push_back(*points.begin());

    smoothedRegion = LineSmoother::DouglasPeucker(smoothedRegion, epsilon);

    smoothedRegion.erase(smoothedRegion.begin());
    smoothedRegion.pop_back();

    points = smoothedRegion;

    return;
  }

  for (size_t i = 0; i < allBorders.size(); i++)
  {
    savedPointsThis.push_back(std::make_pair(allBorders[i].first.fromPoint(), allBorders[i].first.toPoint()));
    savedPointsOthers.push_back(std::make_pair(allBorders[i].second.fromPoint(), allBorders[i].second.toPoint()));

    allContourBorders.insert(allBorders[i].first);

    smoothedLines.push_back(LineBorderVector(LineSmoother::DouglasPeucker(allBorders[i].first.getPoints(), epsilon)));
  }


  auto predLine = std::prev(allContourBorders.end());
  for (auto lineIter = allContourBorders.begin(); lineIter != allContourBorders.end(); ++lineIter)
  {
    savedPointsAlone.push_back(std::make_pair(predLine->toPoint(), lineIter->fromPoint()));

    LineBorder border(*this, predLine->getToIndex(), lineIter->getFromIndex());

    smoothedLinesAlone.push_back(LineBorderVector(LineSmoother::DouglasPeucker(border.getPoints(), epsilon)));

    predLine = lineIter;
  }


  for (size_t i = 0; i < allBorders.size(); i++)
  {
    Contour* other = &allBorders[i].second.getOwner();

    int from = indexOf(savedPointsThis[i].first);
    int to = indexOf(savedPointsThis[i].second);
    LineBorder smoothedBorder1 = LineBorder(*this, from, to);
          
    smoothedBorder1.replaceBorderWith(smoothedLines[i]);

    from = other->indexOf(savedPointsOthers[i].first);
    to = other->indexOf(savedPointsOthers[i].second);
    LineBorder smoothedBorder2 = LineBorder(*other, from, to);
          
    smoothedBorder2.replaceBorderWith(smoothedLines[i]);
  }

  for (size_t i = 0; i < smoothedLinesAlone.size(); i++)
  {
    Contour* other = &allBorders[i].second.getOwner();

    int from = indexOf(savedPointsAlone[i].first);
    int to = indexOf(savedPointsAlone[i].second);

    LineBorder smoothedAloneBorder = LineBorder(*this, from, to);

    smoothedAloneBorder.replaceBorderWith(smoothedLinesAlone[i]);
  }
}