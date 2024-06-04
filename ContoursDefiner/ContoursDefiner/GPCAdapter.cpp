#include "GPCAdapter.h"
#include "GPC/gpc.cpp"
#include "BresenhamLine.h"
#include <algorithm>
#include "StarFixer.h"
#include "CrossFixer.h"

void freePolygon(gpc_polygon* p)
{
  for (int i = 0; i < p->num_contours; i++)
  {
    delete[] p->contour[i].vertex;
  }
  delete[] p->contour;

}


void defineReplaces(const std::list<Contour>& contours, std::vector<GPCFixer*> fixers)
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
    return;
  
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

  for (size_t i = 0; i < fixers.size(); i++)
  {
    fixers[i]->calcReplaces(matrix, XYmin);
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
  std::vector<GPCFixer*> fixers;
  StarFixer star;
  fixers.push_back(&star);

  CrossFixer cross;
  fixers.push_back(&cross);

  defineReplaces(unprocessedContours, fixers);

  std::list<Contour> contours(unprocessedContours);

  for (auto iter = fixers.begin(); iter != fixers.end(); ++iter)
    if (!(*iter)->isEmpty())
      (*iter)->modifyContours(unprocessedContours, contours);

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

  for (auto iter = fixers.rbegin(); iter != fixers.rend(); ++iter)
    if (!(*iter)->isEmpty())
      (*iter)->revertReplaces(holes);

  return holes;
}


