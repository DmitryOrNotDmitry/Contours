#include "BugContoursDefiner.h"

BugContoursDefiner::BugContoursDefiner(AbstractImageData* imageManager)
  : ContourDefiner(imageManager)
{
}

BugContoursDefiner::~BugContoursDefiner()
{
}


void VectorCW(Point& point) // сдвиг вектора ПО часовой стрелке, каждая координата может иметь значения ТОЛЬКО -1, 0 и 1
{
  int& X = point.x;
  int& Y = point.y;

  if (X == 0 && Y == -1) { X = 1; Y = 0; return; }

  if (X == 1 && Y == 0) { X = 0; Y = 1; return; }

  if (X == 0 && Y == 1) { X = -1; Y = 0; return; }

  if (X == -1 && Y == 0) { X = 0; Y = -1; return; }
}

void VectorCCW(Point& point) // сдвиг вектора ПРОТИВ часовой стрелке, каждая координата может иметь значения ТОЛЬКО -1, 0 и 1
{
  int& X = point.x;
  int& Y = point.y;
  
  if (X == 0 && Y == -1) { X = -1; Y = 0; return; }

  if (X == -1 && Y == 0) { X = 0; Y = 1; return; }

  if (X == 0 && Y == 1) { X = 1; Y = 0; return; }

  if (X == 1 && Y == 0) { X = 0; Y = -1; return; }
}


void VectorCW8(Point& point) // сдвиг вектора ПО часовой стрелке (8 направлений)
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

void VectorCCW8(Point& point) // сдвиг вектора ПРОТИВ часовой стрелке (8 направлений)
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



Contour BugContoursDefiner::defineContour(const Point& startPoint)
{
  Contour contour;

  if (imageManager->isContourPoint(startPoint)) // Дана неправильная внутренняя точка
    return contour;

  Point startNearContourPoint = getPointNearContour(startPoint);

  if (startNearContourPoint.x == imageManager->lineSize() - 1) //  Дойдя до правой границы изображения, не найдена точка контура
    return contour;

  // проверяем не заперта ли наша точка со всех четырех сторон (чтобы бесконечно не зациклить основной цикл)
  // ситуация может возникнуть, когда по ошибке задали стартовую точку в области растра картосновы, который залит кодом (254,254,254)
  // точка справа уже проверена, поэтому дополнительно проверяем три оставшиеся точки
  if (imageManager->isContourPoint(startNearContourPoint.toUp())
    && imageManager->isContourPoint(startNearContourPoint.toLeft())
    && imageManager->isContourPoint(startNearContourPoint.toBottom()))
  {
    contour.addPoint(startNearContourPoint.toRight());
    contour.addPoint(startNearContourPoint.toBottom());
    contour.addPoint(startNearContourPoint.toLeft());
    contour.addPoint(startNearContourPoint.toUp());
    return contour;
  }

  Point velocity(1, 0);  // Вектор движения
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

