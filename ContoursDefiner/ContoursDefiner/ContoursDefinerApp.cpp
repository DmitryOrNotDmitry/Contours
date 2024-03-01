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


CContoursDefinerApp::~CContoursDefinerApp()
{
  delete draw;
}

CContoursDefinerApp theApp;

BOOL CContoursDefinerApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}


BOOL CContoursDefinerApp::InitApplication()
{
  __main__();

  return TRUE;
}


void CContoursDefinerApp::__main__()
{
  HIMAGE hImage = DI_ActiveObject;
  ContourDefiner conDefiner = ContourDefiner(hImage);
  
  Point startPoint(187, 195);
  //Point startPoint(4, 4);
  contour = conDefiner.defineContour(startPoint);

  draw = new ContourDrawing(hImage, contour);
}

