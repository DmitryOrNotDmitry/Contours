#pragma once
#include "stdafx.h"
#include "Point.h"

class FirstPointSetting : public CDialog
{
  DECLARE_DYNAMIC(FirstPointSetting)

  enum { IDD = IDD_DIALOG_first_point_setting };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);

  virtual BOOL OnInitDialog();

  void setValueToDlgItem(int dlgItem, int value);

  int getIntFromDlgItem(int dlgItem);

public:
  FirstPointSetting(CWnd* pParent =NULL);
  virtual ~FirstPointSetting();

  DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedOk();

public:

  Point startPoint;

};

