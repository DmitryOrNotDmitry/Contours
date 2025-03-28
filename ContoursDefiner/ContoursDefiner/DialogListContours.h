#pragma once
#include "stdafx.h"
#include "ContourDefiner/Point.h"
#include "ERInterface.h"
#include "ContourState.h"
#include "ContourDefiner/GeneralBorderCalculator.h"
#include "DataStorageManager.h"
#include "ContourDefiner/BresenhamLine.h"
#include "ContourDefiner/GPCAdapter.h"

#include <iterator>
#include <vector>


enum AlgorithmType {
  MY, BUG, IMPROVED_BUG
};

enum AlgorithmStage {
  HOLE_SEPARATE, HOLE_DISTRIBUTION
};


class DialogListContours : public CDialog
{
  DECLARE_DYNAMIC(DialogListContours)

  enum { IDD = IDD_DIALOG_first_point_setting };

  DataStorageManager& dataManager;

  void setValueToDlgItem(int dlgItem, int value);

  int getIntFromDlgItem(int dlgItem) const;
  double getDoubleFromDlgItem(int dlgItem) const;

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
  afx_msg void OnBnClickedShowInitHoles();
  afx_msg void OnBnClickedReset();
  afx_msg void OnBnClickedSmoothContours();
  void OnLvnItemchangedChanlist(NMHDR* pNMHDR, LRESULT* pResult);

  CListCtrl contoursTable;
  CSpinButtonCtrl spinMaxSquare;
  CSpinButtonCtrl spinMinSquare;
  CButton checkBoxShowHoles;

  bool isRowSelected(int row) const;

  std::vector<int> getSelectedRows() const;

  void setContoursStates() const;

public:

  AlgorithmType currentType;
  AlgorithmStage currentStage;

  HIMAGE hImage;

  void addColumn(int fmt, int width, char* capture, int numCol);
  void addRow(int rowNum, CString name);

  void OnRadioButtonClickedAlgorithmDefinerType();
  void OnRadioButtonClickedAlgorithmStageSelect();
};

