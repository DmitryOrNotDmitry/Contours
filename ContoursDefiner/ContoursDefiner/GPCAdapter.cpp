#include "GPCAdapter.h"
#include "GPC/gpc.cpp"
#include "BresenhamLine.h"
#include <algorithm>

void freePolygon(gpc_polygon* p)
{
  for (int i = 0; i < p->num_contours; i++)
  {
    delete[] p->contour[i].vertex;
  }
  delete[] p->contour;

}


struct Replacement
{
  Replacement(Point base, Point inserted) : base(base), inserted(inserted) { }

  Point base;
  Point inserted;
};


std::vector<Point> getBasePoints(const std::list<Contour>& contours)
{
  Point XYmin(1000000000, 1000000000);
  Point XYmax(0, 0);
  int x, y;
  for (auto iter = contours.begin(); iter != contours.end(); ++iter)
  {
    for (size_t i = 0; i < iter->size(); i++)
    {
      x = iter->getPoint(i).x;
      y = iter->getPoint(i).y;

      XYmin.x = std::min(XYmin.x, x);
      XYmin.y = std::min(XYmin.y, y);

      XYmax.x = std::max(XYmax.x, x);
      XYmax.y = std::max(XYmax.y, y);
    }
  }

  std::vector<Point> basePoints;
  
  int rows = XYmax.y - XYmin.y + 2;
  int cols = XYmax.x - XYmin.x + 2;
  if (rows <= 0 || cols <= 0)
    return basePoints;
  
  std::vector<std::vector<char>> matrix;
  matrix.resize(rows);
  for (int i = 0; i < rows; i++)
    matrix[i].resize(cols, 0);

  for (auto iter = contours.begin(); iter != contours.end(); ++iter)
  {
    for (size_t i = 0; i < iter->size(); i++)
    {
      x = iter->getPoint(i).x - XYmin.x + 1;
      y = iter->getPoint(i).y - XYmin.y + 1;

      matrix[y][x]++;
    }
  }

  for (int i = 1; i < rows - 1; i++)
  {
    for (int j = 1; j < cols - 1; j++)
    {
      if (
        matrix[i + 1][j - 1] != 0 && matrix[i + 1][j    ] == 0 && matrix[i + 1][j + 1] != 0 &&
        matrix[i    ][j - 1] == 0 && matrix[i    ][j    ] != 0 && matrix[i    ][j + 1] == 0 &&
        matrix[i - 1][j - 1] != 0 && matrix[i - 1][j    ] == 0 && matrix[i - 1][j + 1] != 0
        )
        basePoints.push_back(Point(j + XYmin.x - 1, i + XYmin.y - 1));
    }
  }

  return basePoints;
}


std::vector<Replacement> modifyContours(const std::list<Contour>& contours, std::list<Contour>& newContours, const std::vector<Point>& basePoints)
{
  std::vector<Replacement> replaces;

  double prevDist, nextDist;
  for (size_t i = 0; i < basePoints.size(); i++)
  {
    Point insertedPoint = basePoints[i].toUp();
    replaces.push_back(Replacement(basePoints[i], insertedPoint));

    for (auto iter = newContours.begin(); iter != newContours.end(); ++iter)
    {
      int baseIdx = iter->indexOf(basePoints[i]);
      if (baseIdx == -1)
        continue;

      int leftIdx = iter->indexOf(insertedPoint.toLeft());
      int rightIdx = iter->indexOf(insertedPoint.toRight());
      if (
        (rightIdx == -1 && leftIdx == -1) ||
        (rightIdx != -1 && leftIdx != -1)
        )
        continue;

      prevDist = insertedPoint.DistanceTo(iter->getPoint(iter->getNextIdx(baseIdx, -1)));
      nextDist = insertedPoint.DistanceTo(iter->getPoint(iter->getNextIdx(baseIdx)));

      if (prevDist < nextDist)
      {
        iter->insertPoint(insertedPoint, baseIdx);
      }
      else
      {
        iter->insertPoint(insertedPoint, baseIdx + 1);
      }
    }
  }

  return replaces;
}


void rollbackReplacements(std::vector<Contour>& holes, const std::vector<Replacement>& replaces)
{
  for (size_t i = 0; i < replaces.size(); i++)
  {
    for (auto iter = holes.begin(); iter != holes.end(); ++iter)
    {
      int insertedIdx = iter->indexOf(replaces[i].inserted);
      if (insertedIdx != -1)
        iter->setPoint(insertedIdx, replaces[i].base);
    }
  }
}


void exportToPolygon(const std::list<Contour>& contours, gpc_polygon* polygon)
{
  size_t countContours = contours.size();
  polygon->contour = new gpc_vertex_list[countContours];
  polygon->num_contours = countContours;

  auto contoursIter = contours.begin();
  for (size_t i = 0; i < countContours; i++)
  {
    gpc_vertex_list& cur_vertex_list = polygon->contour[i];
    cur_vertex_list.num_vertices = contoursIter->size();
    cur_vertex_list.vertex = new gpc_vertex[contoursIter->size()];

    for (size_t j = 0; j < contoursIter->size(); j++)
    {
      cur_vertex_list.vertex[j].x = (*contoursIter)[j].x;
      cur_vertex_list.vertex[j].y = (*contoursIter)[j].y;
    }
    ++contoursIter;
  }
}


std::vector<Contour> importToContours(gpc_polygon* polygon, int isHole = TRUE)
{
  std::vector<Contour> holes;
  holes.reserve(polygon->num_contours);

  for (int i = 0; i < polygon->num_contours; i++)
  {
    if (polygon->hole[i] == isHole)
    {
      Contour hole;
      for (int j = 0; j < polygon->contour[i].num_vertices; j++)
      {
        int x = polygon->contour[i].vertex[j].x;
        int y = polygon->contour[i].vertex[j].y;
        Point nextPoint(x, y);

        if (hole.size() > 0)
        {
          Point lastPoint = hole[hole.size() - 1];
          if (nextPoint.DistanceTo(lastPoint) >= 1.999)
          {
            BresenhamLine connectLine(lastPoint, nextPoint);
            for (size_t k = 0; k < connectLine.size(); k++)
            {
              hole.addPoint(connectLine[k]);
            }
          }
        }

        hole.addPoint(nextPoint);

      }
      holes.push_back(hole);
    }
  }

  return holes;
}


std::vector<Contour> GPCAdapter::searchHoles(const std::list<Contour>& unprocessedContours)
{

  std::vector<Point> basePoints = getBasePoints(unprocessedContours);

  std::list<Contour> contours(unprocessedContours);
  std::vector<Replacement> replaces;

  if (basePoints.size() > 0)
  {
    replaces = modifyContours(unprocessedContours, contours, basePoints);
  }

  gpc_polygon* polygon1 = new gpc_polygon;
  exportToPolygon(contours, polygon1);

  gpc_polygon* polygon2 = new gpc_polygon;
  polygon2->num_contours = 0;


  gpc_polygon* result = new gpc_polygon;
  gpc_polygon_clip(GPC_UNION, polygon1, polygon2, result);


  freePolygon(polygon1);
  delete polygon1;
  delete polygon2;


  std::vector<Contour> holes = importToContours(result);

  gpc_free_polygon(result);
  delete result;

  rollbackReplacements(holes, replaces);

  return holes;
}


