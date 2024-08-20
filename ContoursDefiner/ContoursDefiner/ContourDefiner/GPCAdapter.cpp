#include "GPCAdapter.h"
#include "BresenhamLine.h"
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


std::vector<Contour> importToContours(gpc_polygon* polygon, int isHole = 1)
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
std::pair<std::vector<Contour>, std::vector<Contour>> unionContours(const ContourContainer& contours)
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

  std::vector<Contour> holes = importToContours(result, 1);
  std::vector<Contour> unionContours = importToContours(result, 0);

  gpc_free_polygon(result);
  delete result;

  return std::make_pair(unionContours, holes);
}


std::vector<Contour> GPCAdapter::searchHoles(const std::vector<Contour>& contours)
{
  auto result = unionContours(contours);
  
  std::vector<Contour> unionedContours = std::move(result.first);
  std::vector<Contour> holes = std::move(result.second);

  std::vector<Contour> extraHoles = defineExtraHoles(unionedContours);
  for (size_t i = 0; i < extraHoles.size(); i++)
    if (std::find(holes.begin(), holes.end(), extraHoles[i]) == holes.end())
      holes.push_back(std::move(extraHoles[i]));

  return holes;
}


