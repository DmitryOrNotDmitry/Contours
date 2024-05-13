#pragma once
#include "stdafx.h"
#include "Point.h"
#include <vector>
#include "ERInterface.h"
#include "ContourState.h"
#include "ControlPointsCalculator.h"
#include "DataStorageManager.h"
#include "BresenhamLine.h"
#include "GPCAdapter.h"

class DialogListContours : public CDialog
{
  DECLARE_DYNAMIC(DialogListContours)

  enum { IDD = IDD_DIALOG_first_point_setting };


  DataStorageManager& dataManager;

  void setValueToDlgItem(int dlgItem, int value);

  int getIntFromDlgItem(int dlgItem);

protected:
  virtual void DoDataExchange(CDataExchange* pDX);

  virtual BOOL OnInitDialog();

  
public:
  DialogListContours(CWnd* pParent =NULL);
  virtual ~DialogListContours();

  DECLARE_MESSAGE_MAP()

  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedCalcControlPoints();
  afx_msg void OnBnClickedSearhHoles();
  void OnLvnItemchangedChanlist(NMHDR* pNMHDR, LRESULT* pResult);

  CListCtrl contoursTable;
  CButton buttonControlPoints;

  bool isRowSelected(int row) const;

  std::vector<int> getSelectedRows() const;

  void setContoursStates() const;

public:
  HIMAGE hImage;

  void addColumn(int fmt, int width, char* capture, int numCol);
  void addRow(int rowNum, CString name);

};

