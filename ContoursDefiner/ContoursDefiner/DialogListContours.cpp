#include "DialogListContours.h"
#include "ContourDefiner/LineBorderVector.h"
#include "ContourDefiner/HoleReducer.h"
#include "ContourDefiner/HoleSeparator.h"
#include "ContourDefiner/LineSmoother.h"
#include "ContourDefiner/ContourDefinerFacade.h"
#include "AbstractDrawer.h"



IMPLEMENT_DYNAMIC(DialogListContours, CDialog)

BEGIN_MESSAGE_MAP(DialogListContours, CDialog)
  ON_BN_CLICKED(IDOK, &DialogListContours::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &DialogListContours::OnBnClickedCancel)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTcontours_table, &DialogListContours::OnLvnItemchangedChanlist)
  ON_BN_CLICKED(IDC_CHECKshow_init_holes, &DialogListContours::OnBnClickedShowInitHoles)
  ON_BN_CLICKED(IDC_BUTTONreset, &DialogListContours::OnBnClickedReset)
  ON_BN_CLICKED(IDC_RADIOmy, &DialogListContours::OnRadioButtonClickedAlgorithmDefinerType)
  ON_BN_CLICKED(IDC_RADIObug, &DialogListContours::OnRadioButtonClickedAlgorithmDefinerType)
  ON_BN_CLICKED(IDC_BUTTONnext_stage, &DialogListContours::OnBnClickedButtonNextStage)
END_MESSAGE_MAP()

void DialogListContours::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LISTcontours_table, contoursTable);
  DDX_Control(pDX, IDC_SPINmax_square_distribution, spinMaxSquare);
  DDX_Control(pDX, IDC_SPINmin_square_distribution, spinMinSquare);
  DDX_Control(pDX, IDC_CHECKshow_init_holes, checkBoxShowHoles);
}


DialogListContours::DialogListContours(CWnd* pParent /*=NULL*/)
  : CDialog(DialogListContours::IDD, pParent)
  , dataManager(DataStorageManager::getInstance())
{
  currentType = AlgorithmType::WITH_RECT_ANGLES;
  currentStage = VECTORIZATION;

  groupsBoxForStages.insert(std::make_pair(ProgramStage::VECTORIZATION, IDC_STATICvectorization));
  groupsBoxForStages.insert(std::make_pair(ProgramStage::FIND_AND_PREPARE_HOLES, IDC_STATICfind_holes));
  groupsBoxForStages.insert(std::make_pair(ProgramStage::DISTRIBUTE_HOLES, IDC_STATICdistribute_holes));
  groupsBoxForStages.insert(std::make_pair(ProgramStage::THINNING_CONTOURS, IDC_STATICthinning));
}

DialogListContours::~DialogListContours()
{
}

void DialogListContours::OnBnClickedOk()
{
  CDialog::OnOK();
  this->DestroyWindow();
}

void DialogListContours::OnBnClickedCancel()
{
  this->DestroyWindow();
}

BOOL DialogListContours::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  setCurrentStageCapture(currentStage);

  CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDERprocess);
  pSlider->SetRange(0, 100);
  setSliderPosition(currentStage);

  contoursTable.SetExtendedStyle(this->contoursTable.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

  LVCOLUMN column;
  //Первый столбец - для чекбоксов
  column.fmt = LVCFMT_CENTER;
  column.cx = 20;
  column.iOrder = 0;
  column.mask = LVCF_FMT | LVCF_WIDTH;
  contoursTable.InsertColumn(0, &column);

  addColumn(LVCFMT_LEFT, 130, "Контур (точка скола)", 1);

  SetDlgItemText(IDC_EDITsmooth_epsilon, "1");

  spinMaxSquare.SetRange(0, 1000);
  spinMaxSquare.SetPos(300);
  spinMinSquare.SetRange(0, 1000);
  spinMinSquare.SetPos(4);

  checkBoxShowHoles.SetCheck(BST_CHECKED);

  CheckRadioButton(IDC_RADIOmy, IDC_RADIObug, IDC_RADIOmy);

  CheckDlgButton(IDC_CHECKis_delete_cover_tri, BST_CHECKED);

  return TRUE;
}

CString DialogListContours::getGroupBoxCapture(ProgramStage stage)
{
  CWnd* pGroupBox = GetDlgItem(groupsBoxForStages[stage]);
  if (pGroupBox)
  {
    CString strCaption;
    pGroupBox->GetWindowText(strCaption);
    return strCaption;
  }

  return "Подпись стадии не найдена";
}

void DialogListContours::setCurrentStageCapture(ProgramStage stage)
{
  CString capture = getGroupBoxCapture(stage);

  CWnd* pStatic = GetDlgItem(IDC_STATICcurrent_stage);
  if (pStatic) {
    pStatic->SetWindowText(_T("Текущий этап: " + capture));
  }
}

void DialogListContours::setSliderPosition(ProgramStage stage)
{
  int process = 1;
  int countProcess = 4;

  if (currentStage == VECTORIZATION)
  {
    process = 1;
  }
  else if (currentStage == FIND_AND_PREPARE_HOLES)
  {
    process = 2;
  }
  else if (currentStage == DISTRIBUTE_HOLES)
  {
    process = 3;
  }
  else if (currentStage == THINNING_CONTOURS)
  {
    process = 4;
  }

  CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDERprocess);
  pSlider->SetPos((process - 1) * 100 / (countProcess - 1));
}

void DialogListContours::nextStage()
{
  if (currentStage == VECTORIZATION) 
  {
    currentStage = FIND_AND_PREPARE_HOLES;
    findAndPreparingHoles();
  }
  else if (currentStage == FIND_AND_PREPARE_HOLES)
  {
    currentStage = DISTRIBUTE_HOLES;
    distributeHoles();
  }
  else if (currentStage == DISTRIBUTE_HOLES)
  {
    currentStage = THINNING_CONTOURS;
    thinningContours();
  }

  setCurrentStageCapture(currentStage);
  setSliderPosition(currentStage);
  
  RecalcImageViews(hImage);
}

void DialogListContours::findAndPreparingHoles()
{
  double maxSquare = getDoubleFromDlgItem(IDC_EDITmax_square_distribution);
  double minSquare = getDoubleFromDlgItem(IDC_EDITmin_square_distribution);

  std::vector<Contour*> pContours = dataManager.getPContours();

  std::vector<Contour> dataHoles = GPCAdapter::searchHoles(pContours);

  for (size_t i = 0; i < dataHoles.size(); i++)
  {
    Contour& hole = dataHoles[i];
    double holeArea = hole.area();

    if (holeArea < minSquare)
    {
      dataManager.addHole(std::move(hole));
    }
    else if (holeArea <= maxSquare)
    {
      std::vector<Contour> atomicHoles = HoleSeparator::separateToAtomicParts(hole);

      for (size_t j = 0; j < atomicHoles.size(); j++)
      {
        dataManager.addHole(std::move(atomicHoles[j]));
      }
    }
  }
}

void DialogListContours::distributeHoles()
{
  std::vector<Contour*> pContours = dataManager.getPContours();

  double maxSquare = getDoubleFromDlgItem(IDC_EDITmax_square_distribution);
  double minSquare = getDoubleFromDlgItem(IDC_EDITmin_square_distribution);

  std::vector<Contour> dataHoles = GPCAdapter::searchHoles(pContours);

  dataManager.getHoles().clear();
  for (size_t i = 0; i < dataHoles.size(); i++)
  {
    Contour& hole = dataHoles[i];
    double holeArea = hole.area();

    if (holeArea < minSquare)
    {
      dataManager.addHole(std::move(hole));
    }
    else if (holeArea <= maxSquare)
    {
      std::vector<Contour> atomicHoles = HoleSeparator::separateToAtomicParts(hole);

      for (size_t j = 0; j < atomicHoles.size(); j++)
      {
        dataManager.addHole(std::move(atomicHoles[j]));
      }
    }
  }

  removeHolesBetweenContours(pContours, minSquare, maxSquare);
  
  std::vector<Contour>& allHoles = dataManager.getHoles();
  for (size_t i = 0; i < allHoles.size(); i++)
  {
    Contour& hole = allHoles[i];
    DoublePoint massCenter = AbstractDrawer::massCenter(hole);
    for (size_t j = 0; j < pContours.size(); j++)
    {
      if (pContours[j]->isInner(massCenter))
      {
        dataManager.setHoleOwner(hole, *pContours[j]);
      }
    }
  }
}

void DialogListContours::thinningContours()
{
  const double epsilon = getDoubleFromDlgItem(IDC_EDITsmooth_epsilon);

  std::vector<Contour*> pContours = dataManager.getPContours();

  smoothContours(pContours, epsilon);
}

void DialogListContours::addColumn(int fmt, int width, char* capture, int numCol)
{
  LVCOLUMN column; //Структура для столбцов
  column.fmt = fmt;
  column.cx = width;
  column.pszText = capture;
  column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
  contoursTable.InsertColumn(numCol, &column);
}


void DialogListContours::addRow(int rowNum, CString name)
{
  contoursTable.InsertItem(rowNum, ""); //Добавляем строку
  contoursTable.SetItemText(rowNum, 1, name); //Название канала
  contoursTable.SetCheck(rowNum, true); //По-умолчанию делаем все каналы
}

void DialogListContours::setContoursStates() const
{
  std::list<Contour>& contours = dataManager.getContours();
  auto contoursIter = contours.begin();

  for (int i = 0; i < (int)contours.size(); i++)
  {
    if (!contoursTable.GetCheck(i))
    {
      dataManager.setContourState(*contoursIter, HIDDEN);
    }

    else if (isRowSelected(i))
    {
      dataManager.setContourState(*contoursIter, SELECTED);
    }
    
    else 
    {
      dataManager.setContourState(*contoursIter, VISIBLE);
    }

    ++contoursIter;
  }
}

void DialogListContours::OnBnClickedShowInitHoles()
{
  if (checkBoxShowHoles.GetCheck() == BST_CHECKED)
    dataManager.setShowHoles(true);
  else
    dataManager.setShowHoles(false);

  RecalcImageViews(hImage);
}

void DialogListContours::OnBnClickedReset()
{
  dataManager.getBorders().clear();
  dataManager.getHoles().clear();
  dataManager.getContours().clear();

  contoursTable.DeleteAllItems();

  currentStage = VECTORIZATION;
  setCurrentStageCapture(currentStage);
  setSliderPosition(currentStage);

  RecalcImageViews(hImage);
}

double DialogListContours::getDoubleFromDlgItem(int dlgItem) const
{
  CString str;
  GetDlgItemText(dlgItem, str);
  str.Replace(',', '.');
  return atof(str);
}

void DialogListContours::OnLvnItemchangedChanlist(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  *pResult = 0;

  setContoursStates();

  RecalcImageViews(hImage);
}

bool DialogListContours::isRowSelected(int row) const
{
  UINT nState = contoursTable.GetItemState(row, LVIS_SELECTED);

  if (nState & LVIS_SELECTED)
  {
    return true;
  }

  return false;
}

std::vector<int> DialogListContours::getSelectedRows() const
{
  std::vector<int> selectedRows;
  int numRows = contoursTable.GetItemCount();

  for (int i = 0; i < numRows; i++)
  {
    if (isRowSelected(i))
    {
      selectedRows.push_back(i);
    }
  }

  return selectedRows;
}


void DialogListContours::OnRadioButtonClickedAlgorithmDefinerType()
{
  int checked = GetCheckedRadioButton(IDC_RADIOmy, IDC_RADIObug);

  if (checked == IDC_RADIOmy)
  {
    currentType = AlgorithmType::WITH_RECT_ANGLES;
  }
  else if (checked == IDC_RADIObug)
  {
    currentType = AlgorithmType::BUG;
  }
}

void DialogListContours::OnBnClickedButtonNextStage()
{
  nextStage();
}
