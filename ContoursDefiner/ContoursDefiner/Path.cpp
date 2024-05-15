#include "Path.h"

Path::Path(const std::vector<Point>& pointsToPath)
  : points(pointsToPath)
{
  std::vector<double> distances;
  size_t path_size = points.size();
  distances.resize(path_size);

  for (size_t i = 0; i < path_size; i++)
  {
    distances[i] = points[i].DistanceTo(points[(i + 1) % path_size]);
  }

  double max_dist = -1;
  size_t max_idx = 0;
  for (size_t i = 0; i < path_size; i++)
  {
    if (distances[i] > max_dist)
    {
      max_dist = distances[i];
      max_idx = i;
    }
  }

  if (points.size() > 0)
  {
    for (size_t i = 0; i <= max_idx; i++)
    {
      Point tmp = *points.begin();
      points.erase(points.begin());
      points.push_back(tmp);
    }
  }
}

Path::~Path()
{
}

bool Path::operator==(const Path& other) const
{
  return points == other.points;
}

bool Path::operator==(const std::vector<Point>& vector) const
{
  return points == vector;
}
