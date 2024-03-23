// ContoursDefiner.cpp: ���������� ��������� ������������� ��� DLL.
//
#pragma once

#include "stdafx.h"
#include "ContoursDefinerApp.h"
#include "ContourDefiner.h"
#include "FirstPointSetting.h"
#include "ERImageData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define MOUSE_FILTER_NAME "MouseHandler"


BEGIN_MESSAGE_MAP(CContoursDefinerApp, CWinApp)
END_MESSAGE_MAP()


CContoursDefinerApp::CContoursDefinerApp() 
{
  hImage = DI_ActiveObject;
  imageManager = new ERImageData(hImage);
  conDefiner = ContourDefiner(imageManager);
  contoursDrawer = new ContourDrawing(hImage);

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


void MouseProc(void* pContext,           // ��������
  long MouseFileX,          // ���������� ������� �� �����
  long MouseFileY,          // /-/
  long Operation,           // ��� ��������
                            // WM_MOUSEMOVE, WM_LBUTTONDOWN, WM_LBUTTONUP
  long MouseKeyStatus)      // ���� ������� ������ (��� � ON_MOUSEMOVE() )
{

  if (Operation == WM_LBUTTONDOWN && (MouseKeyStatus & MK_CONTROL))
  {
    Context* pcc = (Context*)pContext;
    Point startPoint = Point(MouseFileX, MouseFileY);

    pcc->app->contour = pcc->app->conDefiner.defineContour(startPoint);

    pcc->app->contoursDrawer->addContour(pcc->app->contour);

    pcc->app->dlg.addRow(pcc->app->contoursDrawer->getCountContours() - 1, "��������� ������");
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


  // �������� �� �������� �������� �������
  if (dlg.Create(IDD_DIALOG_first_point_setting, NULL))
  {
    // ����������� ����������� ����
    dlg.ShowWindow(SW_SHOW);
  }


  /*
  FirstPointSetting dialog;
  dialog.startPoint = Point(187, 195);
 
  if (dialog.DoModal() == IDOK)
  {
    Point startPoint = dialog.startPoint;
    contour = conDefiner.defineContour(startPoint);

    draws.push_back(new ContourDrawing(hImage, contour));
  }
  */
}



