#include "AbstractDrawer.h"


DoublePoint AbstractDrawer::massCenter(Contour& contour) {
  const std::vector<Point>& points = contour.getPoints();

  double cx = 0.0, cy = 0.0;
  double area_sum = 0.0;

  for (size_t i = 0; i < points.size(); ++i) {
    size_t j = (i + 1) % points.size();
    double cross = points[i].x * points[j].y - points[j].x * points[i].y;
    cx += (points[i].x + points[j].x) * cross;
    cy += (points[i].y + points[j].y) * cross;
    area_sum += cross;
  }

  area_sum *= 0.5;
  if (area_sum == 0.0) {
    area_sum = 1;
  }

  cx /= (6.0 * area_sum);
  cy /= (6.0 * area_sum);

  return DoublePoint(cx, cy);
}


void AbstractDrawer::calcNewCoords(DoublePoint massCenter, Point point, double offset, double& x, double& y)
{
  double len = sqrt(pow(massCenter.x - point.x, 2) + pow(massCenter.y - point.y, 2));

  x = point.x + offset * (massCenter.x - point.x) / len;
  y = point.y + offset * (massCenter.y - point.y) / len;
}