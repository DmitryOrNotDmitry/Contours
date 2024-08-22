#include "DialogListContours.h"
#include "ContourDefiner/LineBorderVector.h"
#include "ContourDefiner/HoleReducer.h"
#include "ContourDefiner/HoleSeparator.h"
#include "ContourDefiner/LineSmoother.h"
#include "ContourDefiner/ContourDefinerFacade.h"



IMPLEMENT_DYNAMIC(DialogListContours, CDialog)

BEGIN_MESSAGE_MAP(DialogListContours, CDialog)
  ON_BN_CLICKED(IDOK, &DialogListContours::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &DialogListContours::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_BUTTONcalc_control_points, &DialogListContours::OnBnClickedCalcControlPoints)
  ON_BN_CLICKED(IDC_BUTTONsearch_holes, &DialogListContours::OnBnClickedSearhHoles)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTcontours_table, &DialogListContours::OnLvnItemchangedChanlist)
  ON_BN_CLICKED(IDC_CHECKshow_init_holes, &DialogListContours::OnBnClickedShowInitHoles)
  ON_BN_CLICKED(IDC_BUTTONreset, &DialogListContours::OnBnClickedReset)
  ON_BN_CLICKED(IDC_BUTTONsmooth_contours, &DialogListContours::OnBnClickedSmoothContours)
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

  contoursTable.SetExtendedStyle(this->contoursTable.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

  LVCOLUMN column;
  //Первый столбец - для чекбоксов
  column.fmt = LVCFMT_CENTER;
  column.cx = 20;
  column.iOrder = 0;
  column.mask = LVCF_FMT | LVCF_WIDTH;
  contoursTable.InsertColumn(0, &column);

  addColumn(LVCFMT_LEFT, 170, "Контур", 1);

  spinMaxSquare.SetRange(0, 1000);
  spinMaxSquare.SetPos(300);
  spinMinSquare.SetRange(0, 1000);
  spinMinSquare.SetPos(4);

  checkBoxShowHoles.SetCheck(BST_CHECKED);

  return TRUE;
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


void DialogListContours::OnBnClickedCalcControlPoints()
{
  std::vector<int> selectedRows = getSelectedRows();

  if (selectedRows.size() != 2)
  {
    MessageBox("Необходимо выбрать именно 2 контура", "Построение контуров");
    return;
  }

  std::list<Contour>& conts = dataManager.getContours();

  auto firstCont = std::next(conts.begin(), selectedRows[0]);
  auto secondCont = std::next(conts.begin(), selectedRows[1]);

  std::vector<std::pair<LineBorder, LineBorder>> borders = GeneralBorderCalculator::defineAllGeneralBorders(*firstCont, *secondCont, 0);

  for (size_t i = 0; i < borders.size(); i++)
  {
    dataManager.addBorder(borders[i].first);
    dataManager.addBorder(borders[i].second);
  }

  RecalcImageViews(hImage);
}


void DialogListContours::OnBnClickedSearhHoles()
{
  //std::list<Contour>& listContours1 = dataManager.getContours();

  //std::vector<Contour*> pContours1;
  //for (auto iterCont = listContours1.begin(); iterCont != listContours1.end(); ++iterCont)
  //  pContours1.push_back(&(*iterCont));

  //std::vector<Contour> dataHoles1 = GPCAdapter::searchHoles(pContours1);
  //std::vector<Contour> atomicHoles = HoleSeparator::separateToAtomicParts(dataHoles1[0]);

  //for (size_t i = 0; i < atomicHoles.size(); i++)
  //{
  //  dataManager.addHole(std::move(atomicHoles[i]));
  //}

  //RecalcImageViews(hImage);
  //return;

  if (dataManager.getContours().size() == 0)
  {
    MessageBox("Необходимо создать хотя бы 1 контур!", "Построение контуров");
    return;
  }

  std::list<Contour>& listContours = dataManager.getContours();

  std::vector<Contour*> pContours;
  for (auto iterCont = listContours.begin(); iterCont != listContours.end(); ++iterCont)
    pContours.push_back(&(*iterCont));

  std::vector<Contour> newHoles = GPCAdapter::searchHoles(pContours);
  for (size_t i = 0; i < newHoles.size(); i++)
  {
    dataManager.addHole(std::move(newHoles[i]));
  }

  int maxSquare = getIntFromDlgItem(IDC_EDITmax_square_distribution);
  int minSquare = getIntFromDlgItem(IDC_EDITmin_square_distribution);

  removeHolesBetweenContours(pContours, minSquare, maxSquare);

  RecalcImageViews(hImage);
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

  RecalcImageViews(hImage);
}

void DialogListContours::OnBnClickedSmoothContours()
{
  const double epsilon = getDoubleFromDlgItem(IDC_EDITsmooth_epsilon);

  std::list<Contour>& listContours = dataManager.getContours();

  std::vector<Contour*> pContours;
  for (auto iterCont = listContours.begin(); iterCont != listContours.end(); ++iterCont)
    pContours.push_back(&(*iterCont));

  smoothContours(pContours, epsilon);

  RecalcImageViews(hImage);
}


void DialogListContours::setValueToDlgItem(int dlgItem, int value)
{
  CString str;
  str.Format("%d", value);
  SetDlgItemText(dlgItem, str);
}

int DialogListContours::getIntFromDlgItem(int dlgItem) const
{
  CString str;
  GetDlgItemText(dlgItem, str);
  return atoi(str);
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
