#include "LineSmoother.h"

#include <stack>

double LineSmoother::distanceToLine(const Point& point, const Point& startLine, const Point& endLine)
{
  int x1 = startLine.x;
  int y1 = startLine.y;

  int x2 = endLine.x;
  int y2 = endLine.y;

  double double_area = std::abs((y2 - y1) * point.x - (x2 - x1) * point.y + x2 * y1 - y2 * x1);
  double len = std::sqrt(std::pow((double)(x2 - x1), 2) + std::pow((double)(y2 - y1), 2));
  if (len != 0)
    return double_area / len;

  return 0;
}

std::vector<Point> LineSmoother::DouglasPeucker(std::vector<Point>& points, double epsilon)
{
  if (points.size() < 3)
    return points;

  std::stack<std::pair<int, int>> stack;
  stack.push(std::make_pair(0, points.size() - 1));

  std::vector<bool> keepedPoints(points.size(), true);

  while (!stack.empty())
  {
    int startIdx = stack.top().first;
    int endIdx = stack.top().second;
    stack.pop();

    double maxDist = 0;
    size_t index = startIdx;

    for (size_t i = startIdx + 1; i < endIdx; i++)
    {
      if (keepedPoints[i])
      {
        double dist = distanceToLine(points[i], points[startIdx], points[endIdx]);
        if (dist > maxDist)
        {
          maxDist = dist;
          index = i;
        }
      }
    }

    if (maxDist > epsilon)
    {
      stack.push(std::make_pair(startIdx, index));
      stack.push(std::make_pair(index, endIdx));
    }
    else
    {
      for (size_t i = startIdx + 1; i < endIdx; i++)
        keepedPoints[i] = false;
    }
  }

  std::vector<Point> result;
  result.reserve(points.size());

  for (size_t i = 0; i < keepedPoints.size(); i++)
    if (keepedPoints[i])
      result.push_back(points[i]);

  return result;
}
