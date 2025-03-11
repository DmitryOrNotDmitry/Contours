// ContoursDefiner.h: ������� ���� ��������� ��� DLL ContoursDefiner
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������
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

  ContourDefiner* getContourDefiner();

  ObjectsDrawing* contoursDrawer;

  HIMAGE hImage;
  AbstractImageData* imageManager;
  ContourDefiner* conDefiner;
  DialogListContours* dlg;

  DataStorageManager& dataManager;

  AlgorithmType lastType;

  void __main__();

  DECLARE_MESSAGE_MAP()
};




