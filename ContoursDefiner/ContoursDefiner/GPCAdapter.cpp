#include "GPCAdapter.h"
#include "GPC/gpc.cpp"
#include "BresenhamLine.h"
#include "StarFixer.h"
#include "CrossFixer.h"
#include "GeneralBorderCalculator.h"

#include <algorithm>
#include <iterator>
#include <set>


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

template<class ContourContainer>
void exportToPolygon(const ContourContainer& contours, gpc_polygon* polygon)
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


LineBorder calcHoleBorder(Contour& contour, LineBorder& fitBorder, LineBorder oneLenBorder)
{
  int from = oneLenBorder.getToIndex();
  int to = fitBorder.getFromIndex();
  int distance1 = contour.distance(from, to);
  LineBorder holeBorder(contour, from, to);

  from = fitBorder.getToIndex();
  to = oneLenBorder.getFromIndex();
  int distance2 = contour.distance(from, to);
  if (distance2 < distance1)
  {
    holeBorder = LineBorder(contour, from, to);
  }

  return holeBorder;
}


size_t defineFitBordersIdx(Contour& contour1, Contour& contour2, const std::vector<std::pair<LineBorder, LineBorder>>& allBorders, size_t oneLenBorderIdx, bool isFirstBorder = true)
{
  int minDistance = INT_MAX;
  size_t minBordersIdx = 0;

  const LineBorder* oneLenBorder;
  const LineBorder* curBorder;
  Contour* contour;

  for (int _ = 0; _ < 2; _++)
  {
    if (_ == 0)
    {
      oneLenBorder = &allBorders[oneLenBorderIdx].first;
      contour = &contour1;
    }
    else
    {
      oneLenBorder = &allBorders[oneLenBorderIdx].second;
      contour = &contour2;
    }

    for (size_t i = 0; i < allBorders.size(); i++)
    {
      if (i == oneLenBorderIdx)
        continue;

      if (_ == 0)
        curBorder = &allBorders[i].first;
      else
        curBorder = &allBorders[i].second;


      int from = oneLenBorder->getToIndex();
      int to = curBorder->getFromIndex();
      int distance = contour->distance(from, to);
      if (distance < minDistance)
      {
        minBordersIdx = i;
        minDistance = distance;
      }

      from = curBorder->getToIndex();
      to = oneLenBorder->getFromIndex();
      distance = contour->distance(from, to);
      if (distance < minDistance)
      {
        minBordersIdx = i;
        minDistance = distance;
      }
    }
  }

  return minBordersIdx;
}


std::vector<Contour> defineExtraHoles(std::vector<Contour>& unionContours)
{
  std::vector<Contour> holes;
  std::set<LineBorder> possibleHoles;

  for (auto iter = unionContours.begin(); iter != unionContours.end(); ++iter)
  {
    Contour& contour = *iter;
    for (int i = 0; i < contour.size(); i++)
    {
      int idxSamePoint = contour.indexOf(contour.getPoint(i), contour.getNextIdx(i, 1), contour.size() - 1);

      if (idxSamePoint == -1)
        continue;

      double partArea = contour.area(i, idxSamePoint);

      if (partArea > 0.001)
      {
        int partSize = contour.distance(i, idxSamePoint);
        int startIdx = i;
        if (contour.distance(idxSamePoint, i) < partSize)
        {
          partSize = contour.distance(idxSamePoint, i);
          startIdx = idxSamePoint;
        }

        int endIdx = contour.getNextIdx(startIdx, partSize - 1);

        possibleHoles.insert(LineBorder(contour, startIdx, endIdx));       
      }
    }
  }

  for (auto iter = possibleHoles.begin(); iter != possibleHoles.end(); ++iter)
  {
    Contour newHole;
    
    int idx = iter->getFromIndex();
    for (int j = 0; j < iter->size(); j++)
    {
      newHole.addPoint(iter->getPoint(idx));

      idx = iter->getNextIdx(idx);
    }
    holes.push_back(std::move(newHole));
  }

  

  return holes;
}


template<class ContourContainer>
std::pair<std::vector<Contour>, std::vector<Contour>> unionContours(ContourContainer& contours)
{
  gpc_polygon* polygon1 = new gpc_polygon;
  exportToPolygon(contours, polygon1);

  gpc_polygon* polygon2 = new gpc_polygon;
  polygon2->num_contours = 0;


  gpc_polygon* result = new gpc_polygon;
  gpc_polygon_clip(GPC_UNION, polygon1, polygon2, result);


  freePolygon(polygon1);
  delete polygon1;
  delete polygon2;

  std::vector<Contour> holes = importToContours(result, TRUE);
  std::vector<Contour> unionContours = importToContours(result, FALSE);

  gpc_free_polygon(result);
  delete result;

  return std::make_pair(unionContours, holes);
}


std::vector<Contour> GPCAdapter::searchHoles(const std::list<Contour>& unprocessedContours)
{
  std::list<Contour> contours(unprocessedContours);

  auto result = unionContours(contours);
  
  std::vector<Contour> unionContours = std::move(result.first);
  std::vector<Contour> holes = std::move(result.second);

  std::vector<Contour> extraHoles = defineExtraHoles(unionContours);
  for (size_t i = 0; i < extraHoles.size(); i++)
    if (std::find(holes.begin(), holes.end(), extraHoles[i]) == holes.end())
      holes.push_back(std::move(extraHoles[i]));

  return holes;
}


