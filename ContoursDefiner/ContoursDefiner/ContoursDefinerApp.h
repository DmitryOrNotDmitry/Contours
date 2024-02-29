// ContoursDefiner.h: главный файл заголовка для DLL ContoursDefiner
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы
#include "ERInterface.h"

class CVDirectDrawingRect : public CVDirectDrawing
{
public:
  void OnDraw(HDC hDC);
  virtual ~CVDirectDrawingRect();
};

class CContoursDefinerApp : public CWinApp
{
public:
	CContoursDefinerApp();

public:
	virtual BOOL InitApplication();
  virtual BOOL InitInstance();

  CVDirectDrawingRect cVDDrect;
	
  DECLARE_MESSAGE_MAP()
};




