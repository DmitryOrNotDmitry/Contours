// ContoursDefiner.cpp: определяет процедуры инициализации для DLL.
//

#include "stdafx.h"
#include "ContoursDefinerApp.h"
#include "ContourDefiner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CContoursDefinerApp, CWinApp)
END_MESSAGE_MAP()

CContoursDefinerApp::CContoursDefinerApp()
{
}

CContoursDefinerApp theApp;

BOOL CContoursDefinerApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}


BOOL CContoursDefinerApp::InitApplication()
{
 /* cVDDrect = CVDirectDrawingRect();

  cVDDrect.Attach(DI_ActiveObject);

  cVDDrect.Update();

  RecalcImageViews(DI_ActiveObject);*/
  
  ContourDefiner conDefiner = ContourDefiner(DI_ActiveObject);
  conDefiner.main();

  return TRUE;
}


CVDirectDrawingRect::~CVDirectDrawingRect()
{
  Detach();
}

void CVDirectDrawingRect::OnDraw(HDC hDC)
{
  HGDIOBJ oldPen = SelectObject(hDC, CreatePen(PS_SOLID, 1, RGB(0, 255, 0)));

  POINT points[] = { {10, 10}, {50, 10}, {50, 50}, {10, 60} }; // Замкнутый многоугольник
  int numPoints = sizeof(points) / sizeof(points[0]);
  Polygon(hDC, points, numPoints);
  
  SelectObject(hDC, oldPen);
}