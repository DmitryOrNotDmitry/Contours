// ContoursDefiner.h: ������� ���� ��������� ��� DLL ContoursDefiner
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������
#include "ERInterface.h"
#include "ContourDrawing.h"
#include "Contour.h"
#include "ContourDefiner.h"
#include "ImageDataManager.h"


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

  void __main__();

  DECLARE_MESSAGE_MAP()
};




