#pragma once
#include "Contour.h"
#include "ERImageData.h"

// ������� ������� �� ������ ��������� ���������� ����� ��������, hImage - �����
std::vector<Contour> createContours(const std::vector<Point>& insideContoursPoints, HIMAGE hImage);

std::vector<Contour> createContours(const std::vector<Point>& insideContoursPoints, AbstractImageData* imageData);


// ������� ���������� ������� ("�����") ����� ���������.
// ����� � �������� �� 0 �� minSquare �������������� � ������ ������� � ���������� ��������,
// �� minSquare �� maxSquare �������������� ���������� ����� ���������,
// �� maxSquare �� ���������� � �������������
void removeHolesBetweenContours(std::vector<Contour>& contours, double minSquare, double maxSquare);

// ������������ ��������� �������� �������� � ������� ��������� �������-������
void smoothContours(std::vector<Contour>& contours, const double epsilon);
