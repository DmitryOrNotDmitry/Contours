// ContoursDefiner.cpp: определяет процедуры инициализации для DLL.
//
#pragma once

#include "stdafx.h"
#include "ContoursDefinerApp.h"
#include "ContourDefiner/ContourDefiner.h"
#include "DialogListContours.h"
#include "ContourDefiner/ERImageData.h"
#include "ContourDefiner/GeneralBorderCalculator.h"
#include "ContourDefiner/ImprovedBugContourDefiner.h"
#include "ContourDefiner/BugContourDefiner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define MOUSE_FILTER_NAME "MouseHandler"


BEGIN_MESSAGE_MAP(CContoursDefinerApp, CWinApp)
END_MESSAGE_MAP()


CContoursDefinerApp::CContoursDefinerApp() 
  : dataManager(DataStorageManager::getInstance())
{
  hImage = DI_ActiveObject;
  imageManager = new ERImageData(hImage);
  dlg = new DialogListContours();
  conDefiner = new ContourDefiner(imageManager);
}


CContoursDefinerApp::~CContoursDefinerApp()
{
  if (imageManager)
    delete imageManager;
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


struct Context
{
  CContoursDefinerApp* app;
};


ContourDefiner* CContoursDefinerApp::getContourDefiner()
{
  if (lastType == dlg->currentType) {
    return conDefiner;
  }

  lastType = dlg->currentType;

  delete conDefiner;

  if (dlg->currentType == AlgorithmType::WITH_RECT_ANGLES)
    conDefiner = new ContourDefiner(imageManager);

  if (dlg->currentType == AlgorithmType::BUG)
    conDefiner = new BugContourDefiner(imageManager);


  return conDefiner;
}


void MouseProc(void* pContext,            // Контекст
                long MouseFileX,          // Координаты курсора по файлу
                long MouseFileY,          // /-/
                long Operation,           // Тип операции
                                          // WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_LBUTTONUP
                long MouseKeyStatus)      // Флаг нажатых кнопок (как в ON_MOUSEMOVE() )
  {

  if (Operation == WM_LBUTTONDOWN && (MouseKeyStatus & MK_CONTROL))
  {
    Context* pcc = (Context*)pContext;
    Point startPoint = Point(MouseFileX, MouseFileY);

    CContoursDefinerApp* app = pcc->app;

    Contour contour = app->getContourDefiner()->defineContour(startPoint);
    app->dataManager.addContour(contour);

    CString name;
    name.Format("Контур (%d, %d)", startPoint.x, startPoint.y);
    app->dlg->addRow(app->dataManager.getCountContours() - 1, name);
  }
}


void ReleaseContext(void* pContext)
{
  Context* pCC = (Context*)pContext;

  delete pCC;
}


void CContoursDefinerApp::__main__()
{
  Context* pCC = new Context;
  pCC->app = this;

  char* cFilterName = MOUSE_FILTER_NAME;

  AddRasterFilterEx(hImage, pCC, m_hInstance, cFilterName);
  SetRasterFilterEx(hImage, pCC, cFilterName, SRF_MouseProc, MouseProc);
  SetRasterFilterEx(hImage, pCC, cFilterName, SRF_ReleaseProc, ReleaseContext);

  CWnd* pMainWnd = CWnd::FromHandle(NS_MAIN_WND);
  contoursDrawer = new ObjectsDrawing(hImage, *dlg);

  if (dlg->Create(IDD_DIALOG_first_point_setting, pMainWnd))
  {
    dlg->hImage = hImage;

    dlg->ShowWindow(SW_NORMAL);
    dlg->UpdateWindow();
  }
}



