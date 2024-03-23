// ContoursDefiner.h: главный файл заголовка для DLL ContoursDefiner
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы
#include "ERInterface.h"
#include "ContourDrawing.h"
#include "Contour.h"
#include "ContourDefiner.h"
#include "ImageDataManager.h"
#include "FirstPointSetting.h"


class CContoursDefinerApp : public CWinApp
{
public:
	CContoursDefinerApp();
  ~CContoursDefinerApp();


	virtual BOOL InitApplication();
  virtual BOOL InitInstance();

  ContourDrawing* contoursDrawer;
  Contour contour;
  HIMAGE hImage;
  ImageDataManager* imageManager;
  ContourDefiner conDefiner;

  FirstPointSetting dlg;

  void __main__();

  DECLARE_MESSAGE_MAP()
};




