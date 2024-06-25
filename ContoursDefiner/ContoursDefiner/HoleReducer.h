#pragma once
#include "Contour.h"

class HoleReducer
{

  double minSquare;
  double maxSquare;

  Contour* reducedHole;
  std::vector<Contour*> nearbyContours;

  const double limitDistance;


  void reduceHoleMultiBorders();

  void prepareContours(std::list<Contour>& contours);

  void includeIntoDominant(Contour& hole);

  Contour* getContourWithMaxBorder(Contour& hole, std::vector<Contour*> contours);

public:

  HoleReducer(double minSquare, double maxSquare);

  void processMulti(Contour& hole, std::list<Contour>& contours);



private:
  // DEPRECATED - for the dominant contour
  void process(Contour& hole, std::list<Contour>& contours);

  void reduceHole(Contour& hole, std::list<Contour>& contours);

  int givePartHoleToContour(Contour& hole, Contour& contour);
  // DEPRECATED - end
};

