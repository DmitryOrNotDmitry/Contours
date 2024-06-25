#pragma once
#include "Contour.h"

#include <map>
#include <set>

struct HoleDistribution
{
  HoleDistribution() : hole(nullptr), forContour(nullptr) {}

  HoleDistribution(Contour* hole, Contour* forContour)
    : hole(hole), forContour(forContour) { }

  Contour* hole;
  Contour* forContour;
};



class HoleReducer
{
  double minSquare;
  double maxSquare;

  Contour* reducedHole;
  std::vector<Contour*> nearbyContours;

  const double limitDistance;

  std::multimap<int, HoleDistribution> holesDistribution;
  std::map<Contour*, int> countHolesForContour;
  std::set<Contour*> skippedContours;


  void reduceHoleMultiBorders();

  void prepareContours(std::list<Contour>& contours);

  void includeIntoDominant(Contour& hole);

  Contour* getContourWithMaxBorder(Contour& hole, std::vector<Contour*> contours);

  void distributeHolesToContours(std::vector<Contour>& holes);

  void defineSkippedContours(std::vector<Contour>& holes);

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

