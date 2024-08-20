// ContoursDefiner.h: главный файл заголовка для DLL ContoursDefiner
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы
#include "ERInterface.h"
#include "ObjectsDrawing.h"
#include "ContourDefiner/Contour.h"
#include "ContourDefiner/ContourDefiner.h"
#include "ContourDefiner/AbstractImageData.h"
#include "DialogListContours.h"
#include "DataStorageManager.h"


class CContoursDefinerApp : public CWinApp
{
public:
	CContoursDefinerApp();
  ~CContoursDefinerApp();


	virtual BOOL InitApplication();
  virtual BOOL InitInstance();

  ObjectsDrawing* contoursDrawer;
  Contour contour;
  HIMAGE hImage;
  AbstractImageData* imageManager;
  ContourDefiner conDefiner;
  DataStorageManager& dataManager;


  void __main__();

  DECLARE_MESSAGE_MAP()
};




