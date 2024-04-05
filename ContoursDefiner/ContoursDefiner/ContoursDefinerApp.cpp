// ContoursDefiner.cpp: определяет процедуры инициализации для DLL.
//
#pragma once

#include "stdafx.h"
#include "ContoursDefinerApp.h"
#include "ContourDefiner.h"
#include "DialogListContours.h"
#include "ERImageData.h"
#include "ControlPointsCalculator.h"

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
  conDefiner = ContourDefiner(imageManager);
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
  DialogListContours* dlg;
  
  CContoursDefinerApp* app;
};


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

    pcc->app->contour = pcc->app->conDefiner.defineContour(startPoint);

    pcc->app->dataManager.addContour(pcc->app->contour);

    CString name;
    name.Format("Контур (%d, %d)", startPoint.x, startPoint.y);
    pcc->dlg->addRow(pcc->app->dataManager.getCountContours() - 1, name);
  }
}


void ReleaseContext(void* pContext)
{
  Context* pCC = (Context*)pContext;

  delete pCC->dlg;
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
  pCC->dlg = new DialogListContours();
  contoursDrawer = new ContourDrawing(hImage, *pCC->dlg);
  if (pCC->dlg->Create(IDD_DIALOG_first_point_setting, pMainWnd))
  {
    pCC->dlg->hImage = hImage;

    pCC->dlg->ShowWindow(SW_NORMAL);
    pCC->dlg->UpdateWindow();
  }
}



