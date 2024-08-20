#include "ContourDefinerFacade.h"

#include "ContourDefiner.h"
#include "GPCAdapter.h"
#include "HoleReducer.h"

std::vector<Contour> createContours(const std::vector<Point>& insideContoursPoints, HIMAGE hImage)
{
  std::vector<Contour> contours;

  ERImageData imageData(hImage);
  ContourDefiner definer(&imageData);

  for (auto iterPoint = insideContoursPoints.begin(); iterPoint != insideContoursPoints.end(); ++iterPoint)
  {
    contours.push_back(std::move(definer.defineContour(*iterPoint)));
  }

  return contours;
}

void removeHolesBetweenContours(std::vector<Contour>& contours, double minSquare, double maxSquare)
{
  std::vector<Contour> holes = GPCAdapter::searchHoles(contours);

  HoleReducer holeReducer(minSquare, maxSquare);

  for (size_t i = 0; i < holes.size(); i++)
  {
    holeReducer.processMulti(holes[i], contours);
  }
}

void smoothContours(std::vector<Contour>& contours, const double epsilon)
{
  for (auto contour = contours.begin(); contour != contours.end(); ++contour)
  {
    contour->smooth(epsilon, contours);
  }
}
