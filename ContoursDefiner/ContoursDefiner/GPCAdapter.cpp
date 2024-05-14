#include "GPCAdapter.h"
#include "GPC/gpc.cpp"


void freePolygon(gpc_polygon* p)
{
  for (int i = 0; i < p->num_contours; i++)
  {
    delete[] p->contour[i].vertex;
  }
  delete[] p->contour;

}


std::vector<Contour> GPCAdapter::searchHoles(const std::list<Contour>& contours)
{

  gpc_polygon* polygon1 = new gpc_polygon;
  size_t countContours = contours.size();
  polygon1->contour = new gpc_vertex_list[countContours];
  polygon1->num_contours = countContours;

  auto contoursIter = contours.begin();
  for (size_t i = 0; i < countContours; i++)
  {
    gpc_vertex_list& cur_vertex_list = polygon1->contour[i];
    cur_vertex_list.num_vertices = contoursIter->size();
    cur_vertex_list.vertex = new gpc_vertex[contoursIter->size()];

    for (size_t j = 0; j < contoursIter->size(); j++)
    {
      cur_vertex_list.vertex[j].x = (*contoursIter)[j].x;
      cur_vertex_list.vertex[j].y = (*contoursIter)[j].y;
    }
    ++contoursIter;
  }


  gpc_polygon* polygon2 = new gpc_polygon;
  polygon2->num_contours = 0;


  gpc_polygon* result = new gpc_polygon;
  gpc_polygon_clip(GPC_UNION, polygon1, polygon2, result);

  freePolygon(polygon1);
  delete polygon1;
  delete polygon2;

  std::vector<Contour> holes;
  holes.reserve(result->num_contours);

  for (int i = 0; i < result->num_contours; i++)
  {
    if (result->hole[i] == TRUE)
    {
      Contour hole;
      for (int j = 0; j < result->contour[i].num_vertices; j++)
      {
        int x = result->contour[i].vertex[j].x;
        int y = result->contour[i].vertex[j].y;
        hole.addPoint(Point(x, y));
      }
      holes.push_back(hole);
    }
  }

  gpc_free_polygon(result);

  return holes;
}


