#include "HoleSeparator.h"

DoublePoint getCentroid(std::vector<Point>& points) 
{
  double sumX = 0.0, sumY = 0.0;
  int count = points.size();

  for (int i = 0; i < points.size(); i++) 
  {
    sumX += static_cast<double>(points[i].x);
    sumY += static_cast<double>(points[i].y);
  }

  DoublePoint p;
  p.x = sumX / count;
  p.y = sumY / count;
  return p;
}


std::vector<Contour> HoleSeparator::separateToAtomicParts(const Contour& hole)
{
  std::vector<Contour> atomicHoles;

  Point XYmin(1000000000, 1000000000);
  Point XYmax(0, 0);
  int x, y;
  for (size_t i = 0; i < hole.size(); i++)
  {
    x = hole.getPoint(i).x;
    y = hole.getPoint(i).y;

    XYmin.x = std::min(XYmin.x, x);
    XYmin.y = std::min(XYmin.y, y);

    XYmax.x = std::max(XYmax.x, x);
    XYmax.y = std::max(XYmax.y, y);
  }

  int rows = XYmax.y - XYmin.y + 3;
  int cols = XYmax.x - XYmin.x + 3;
  if (rows <= 0 || cols <= 0)
    return atomicHoles;

  std::vector<std::vector<char>> matrix;
  matrix.resize(rows);
  for (int i = 0; i < rows; i++)
    matrix[i].resize(cols, 0);

  for (size_t i = 0; i < hole.size(); i++)
  {
    x = hole.getPoint(i).x - XYmin.x + 1;
    y = hole.getPoint(i).y - XYmin.y + 1;

    matrix[y][x] = 1;
  }

  const int yOffset = XYmin.y - 1;
  const int xOffset = XYmin.x - 1;
  
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      Point p(j + xOffset, i + yOffset);
      if (hole.isInner(p))
        matrix[i][j] = 1;
    }
  }
  
  std::vector<Point> fitPoints;
  fitPoints.reserve(4);

  for (int i = 1; i < rows - 2; i++)
  {
    for (int j = 1; j < cols - 2; j++)
    {
      Point p1(j + xOffset, i + yOffset);
      Point p2 = p1.toRight();
      Point p3 = p2.toBottom();
      Point p4 = p3.toLeft();

      fitPoints.clear();

      if (matrix[i][j] == 1)
      {
        fitPoints.push_back(Point(j + xOffset, i + yOffset));
      }

      if (matrix[i][j + 1] == 1)
      {
        fitPoints.push_back(Point((j + 1) + xOffset, (i) + yOffset));
      }

      if (matrix[i + 1][j + 1] == 1)
      {
        fitPoints.push_back(Point((j + 1) + xOffset, (i + 1) + yOffset));
      }

      if (matrix[i + 1][j] == 1)
      {
        fitPoints.push_back(Point((j) + xOffset, (i + 1) + yOffset));
      }

      if (fitPoints.size() == 3)
      {
        DoublePoint triangleCentroid = getCentroid(fitPoints);

        if (!hole.isInner(triangleCentroid))
          continue;
      }

      if (fitPoints.size() > 2)
      {
        Contour newHole;
        for (size_t k = 0; k < fitPoints.size(); k++)
          newHole.addPoint(fitPoints[k]);

        atomicHoles.push_back(newHole);
      }

    }
  }


  return atomicHoles;
}
