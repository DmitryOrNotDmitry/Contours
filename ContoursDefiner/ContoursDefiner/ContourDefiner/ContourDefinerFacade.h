#pragma once
#include "Contour.h"
#include "ERImageData.h"

// —оздает контуры на основе начальных внутренних точек контуров, hImage - растр
std::vector<Contour> createContours(const std::vector<Point>& insideContoursPoints, HIMAGE hImage);

std::vector<Contour> createContours(const std::vector<Point>& insideContoursPoints, AbstractImageData* imageData);


// ”дал€ет внутренние контура ("дырки") между контурами.
// ƒырки с площадью от 0 до minSquare распредел€ютс€ в пользу контура с наибольшей границей,
// от minSquare до maxSquare распредел€ютс€ равномерно между контурами,
// от maxSquare не учавствуют в распределении
void removeHolesBetweenContours(std::vector<Contour>& contours, double minSquare, double maxSquare);

// ќптимизирует векторное описание контуров с помощью алгоритма ƒугласа-ѕекера
void smoothContours(std::vector<Contour>& contours, const double epsilon);
