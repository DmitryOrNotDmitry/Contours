#pragma once
#include "Contour.h"
#include "ERImageData.h"


/*
  ��� ������ ������ ����������, �����
    
    1. ���� "ERInterface.h" ��������� �� 1-�� ������ � ������ ������ "ContourDefiner"
    
    2. ���� ���������� ���������� GPC, ������ ����� ���������� ������ ���������� "GPC"
    � ���������� �� 1-�� ������ � ������ ������ "ContourDefiner"
*/

// ������� ������� �� ������ ��������� ���������� ����� ��������, hImage - �����
std::vector<Contour> createContours(const std::vector<Point>& insideContoursPoints, HIMAGE hImage);

std::vector<Contour> createContours(const std::vector<Point>& insideContoursPoints, AbstractImageData* imageData);


// ������� ���������� ������� ("�����") ����� ��������� contours.
// ����� � �������� �� 0 �� minSquare �������������� � ������ ������� � ���������� ��������,
// �� minSquare �� maxSquare �������������� ���������� ����� ���������,
// �� maxSquare �� ���������� � �������������
void removeHolesBetweenContours(std::vector<Contour>& contours, double minSquare, double maxSquare);

void removeHolesBetweenContours(std::vector<Contour*>& contours, double minSquare, double maxSquare);


// ������������ ��������� �������� �������� � ������� ��������� �������-������
void smoothContours(std::vector<Contour>& contours, const double epsilon);

void smoothContours(std::vector<Contour*>& contours, const double epsilon);
