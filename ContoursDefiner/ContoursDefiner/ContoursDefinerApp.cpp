// ContoursDefiner.cpp: определяет процедуры инициализации для DLL.
//

#include "stdafx.h"
#include "ContoursDefinerApp.h"
#include "ContourDefiner.h"
#include "FirstPointSetting.h"
#include "ERImageData.h"

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
  ImageDataManager& imageManager = ERImageData(hImage);
  ContourDefiner conDefiner = ContourDefiner(imageManager);

  FirstPointSetting dialog;
  dialog.startPoint = Point(187, 195);
  if (dialog.DoModal() == IDOK)
  {
    Point startPoint = dialog.startPoint;
    contour = conDefiner.defineContour(startPoint);

    draws.push_back(new ContourDrawing(hImage, contour));
  }
}

