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
  WITH_RECT_ANGLES, BUG
};

enum ProgramStage {
  VECTORIZATION,
  FIND_AND_PREPARE_HOLES,
  DISTRIBUTE_HOLES,
  THINNING_CONTOURS
};


class DialogListContours : public CDialog
{
  DECLARE_DYNAMIC(DialogListContours)

  enum { IDD = IDD_DIALOG_first_point_setting };

  DataStorageManager& dataManager;

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
  afx_msg void OnBnClickedShowInitHoles();
  afx_msg void OnBnClickedReset();
  afx_msg void OnBnClickedButtonNextStage();

  void OnLvnItemchangedChanlist(NMHDR* pNMHDR, LRESULT* pResult);
  void OnRadioButtonClickedAlgorithmDefinerType();

  CListCtrl contoursTable;
  CSpinButtonCtrl spinMaxSquare;
  CSpinButtonCtrl spinMinSquare;
  CButton checkBoxShowHoles;

  bool isRowSelected(int row) const;

  std::vector<int> getSelectedRows() const;

  void setContoursStates() const;

public:

  AlgorithmType currentType;

  std::map<ProgramStage, int> groupsBoxForStages;
  ProgramStage currentStage;

  CString getGroupBoxCapture(ProgramStage stage);
  void setCurrentStageCapture(ProgramStage stage);
  void setSliderPosition(ProgramStage stage);

  void nextStage();

  void findAndPreparingHoles();
  void distributeHoles();
  void thinningContours();

  HIMAGE hImage;

  void addColumn(int fmt, int width, char* capture, int numCol);
  void addRow(int rowNum, CString name);
  afx_msg void OnBnClickedCheckOffsetContours();
};

