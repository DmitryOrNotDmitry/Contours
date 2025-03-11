#include "ImprovedBugContourDefiner.h"

ImprovedBugContourDefiner::ImprovedBugContourDefiner(AbstractImageData* imageManager)
  : BugContourDefiner(imageManager)
{
}

ImprovedBugContourDefiner::~ImprovedBugContourDefiner()
{
}


void VectorCW8(Point& point) // ����� ������� �� ������� ������� (8 �����������)
{
  int& X = point.x;
  int& Y = point.y;

  if (X == 0 && Y == -1) { X = 1; Y = -1; return; }
  if (X == 1 && Y == -1) { X = 1; Y = 0; return; }
  if (X == 1 && Y == 0) { X = 1; Y = 1; return; }
  if (X == 1 && Y == 1) { X = 0; Y = 1; return; }
  if (X == 0 && Y == 1) { X = -1; Y = 1; return; }
  if (X == -1 && Y == 1) { X = -1; Y = 0; return; }
  if (X == -1 && Y == 0) { X = -1; Y = -1; return; }
  if (X == -1 && Y == -1) { X = 0; Y = -1; return; }
}

void VectorCCW8(Point& point) // ����� ������� ������ ������� ������� (8 �����������)
{
  int& X = point.x;
  int& Y = point.y;

  if (X == 0 && Y == -1) { X = -1; Y = -1; return; }
  if (X == -1 && Y == -1) { X = -1; Y = 0; return; }
  if (X == -1 && Y == 0) { X = -1; Y = 1; return; }
  if (X == -1 && Y == 1) { X = 0; Y = 1; return; }
  if (X == 0 && Y == 1) { X = 1; Y = 1; return; }
  if (X == 1 && Y == 1) { X = 1; Y = 0; return; }
  if (X == 1 && Y == 0) { X = 1; Y = -1; return; }
  if (X == 1 && Y == -1) { X = 0; Y = -1; return; }
}



Contour ImprovedBugContourDefiner::defineContour(const Point& startPoint)
{
  Contour contour;

  if (imageManager->isContourPoint(startPoint)) // ���� ������������ ���������� �����
    return contour;

  Point startNearContourPoint = getPointNearContour(startPoint);

  if (startNearContourPoint.x == imageManager->lineSize() - 1) //  ����� �� ������ ������� �����������, �� ������� ����� �������
    return contour;

  // ��������� �� ������� �� ���� ����� �� ���� ������� ������ (����� ���������� �� ��������� �������� ����)
  // �������� ����� ����������, ����� �� ������ ������ ��������� ����� � ������� ������ ����������, ������� ����� ����� (254,254,254)
  // ����� ������ ��� ���������, ������� ������������� ��������� ��� ���������� �����
  if (imageManager->isContourPoint(startNearContourPoint.toUp())
    && imageManager->isContourPoint(startNearContourPoint.toLeft())
    && imageManager->isContourPoint(startNearContourPoint.toBottom()))
  {
    contour.addPoint(startNearContourPoint.toRight());
    contour.addPoint(startNearContourPoint.toRight().toBottom());
    contour.addPoint(startNearContourPoint.toBottom());
    contour.addPoint(startNearContourPoint.toBottom().toLeft());
    contour.addPoint(startNearContourPoint.toLeft());
    contour.addPoint(startNearContourPoint.toLeft().toUp());
    contour.addPoint(startNearContourPoint.toUp());
    contour.addPoint(startNearContourPoint.toUp().toRight());

    return contour;
  }

  Point velocity(1, 0);  // ������ ��������
  Point start = startNearContourPoint;
  Point FX = start + velocity;
  Point prev(-1, -1);
  bool IsFirstIteration = true;

  Point currentPoint = start;

  while (true)
  {
    if (!IsFirstIteration && start == currentPoint && FX == currentPoint + velocity)
      break;
    IsFirstIteration = false;

    Point checkedPoint = currentPoint + velocity;

    if (imageManager->isContourPoint(checkedPoint)) 
    {
      if (prev != checkedPoint) 
      {
        contour.addPoint(checkedPoint);
        prev = checkedPoint;
      }
      Point velocity8 = velocity;

      VectorCCW8(velocity8);
      checkedPoint = currentPoint + velocity8;
      if (imageManager->isContourPoint(checkedPoint))
        contour.addPoint(checkedPoint);

      VectorCCW(velocity);
      continue;
    }

    currentPoint += velocity;
    VectorCW(velocity);
  }

  contour.removeSamePointsAtEnds();

  return contour;
}

