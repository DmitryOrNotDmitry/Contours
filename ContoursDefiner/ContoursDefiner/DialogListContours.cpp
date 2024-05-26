#include "DialogListContours.h"

IMPLEMENT_DYNAMIC(DialogListContours, CDialog)

BEGIN_MESSAGE_MAP(DialogListContours, CDialog)
  ON_BN_CLICKED(IDOK, &DialogListContours::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &DialogListContours::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_BUTTONcalc_control_points, &DialogListContours::OnBnClickedCalcControlPoints)
  ON_BN_CLICKED(IDC_BUTTONsearch_holes, &DialogListContours::OnBnClickedSearhHoles)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTcontours_table, &DialogListContours::OnLvnItemchangedChanlist)
END_MESSAGE_MAP()

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

  std::pair<LineBorder, LineBorder> borders = GeneralBorderCalculator::defineNearBorders(*firstCont, *secondCont, 6);

  if (borders.first.size() > 5)
  {
    borders.second.replaceBorderWith(borders.first);

    firstCont->haveRepeatPoints();
    secondCont->haveRepeatPoints();

    dataManager.addBorder(borders.first);
    dataManager.addBorder(borders.second);
  }
  else
  {
    MessageBox("Слишком маленькая общая граница между контурами. Объединение отменено!", "Построение контуров");
  }

  RecalcImageViews(hImage);
}

void DialogListContours::OnBnClickedSearhHoles()
{
  if (dataManager.getContours().size() == 0)
  {
    MessageBox("Необходимо создать хотя бы 1 контур!", "Построение контуров");
    return;
  }

  std::vector<Contour> dataHoles = GPCAdapter::searchHoles(dataManager.getContours());
  int countNewHoles = dataHoles.size();
  for (size_t i = 0; i < dataHoles.size(); i++)
  {
    dataManager.addHole(std::move(dataHoles[i]));
  }
  
  
  std::vector<Contour>& holes = dataManager.getHoles();

  std::list<Contour>& contours = dataManager.getContours();
  for (size_t i = holes.size() - countNewHoles; i < holes.size(); i++)
  {
    std::vector<Contour*> contsWithGeneralBoreder = holes[i].calcNeighbors(contours);

    Contour* contWithMaxBorder = nullptr;
    int maxBorderSize = 0;

    double limitDistance = 1;

    /*if (holes[i].area() <= 8)
      continue;*/

    for (size_t k = 0; k < contsWithGeneralBoreder.size(); k++)
    {
      auto borders = GeneralBorderCalculator::defineNearBorders(holes[i], *contsWithGeneralBoreder[k], limitDistance);
      if (borders.second.size() > maxBorderSize)
      {
        contWithMaxBorder = contsWithGeneralBoreder[k];
        maxBorderSize = borders.first.size();
      }

      //dataManager.addBorder(borders.second);

    }

    if (contWithMaxBorder)
    {
      auto borders = GeneralBorderCalculator::defineNearBorders(holes[i], *contWithMaxBorder, limitDistance);

//      dataManager.addBorder(borders.first);
      dataManager.addBorder(borders.second);
    }

    Point p = holes[i].getAvaragePoint();
    CString s;
    s.Format("Средняя точка : (%d, %d), имеет контуров в соседях: %d, максимальная граница: %d", p.x, p.y, contsWithGeneralBoreder.size(), maxBorderSize);
    //MessageBox(s, "Построение контуров");
  }

  

  RecalcImageViews(hImage);
}

void DialogListContours::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LISTcontours_table, contoursTable);
  DDX_Control(pDX, IDC_BUTTONcalc_control_points, buttonControlPoints);
}

BOOL DialogListContours::OnInitDialog()
{
  CDialog::OnInitDialog();

  contoursTable.SetExtendedStyle(this->contoursTable.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

  LVCOLUMN column; //Структура для столбцов

  //Первый столбец - для чекбоксов
  column.fmt = LVCFMT_CENTER;
  column.cx = 20;
  column.iOrder = 0;
  column.mask = LVCF_FMT | LVCF_WIDTH;
  contoursTable.InsertColumn(0, &column);


  addColumn(LVCFMT_LEFT, 170, "Контур", 1);

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
      contoursIter->setState(HIDDEN);
    }

    else if (isRowSelected(i))
    {
      contoursIter->setState(SELECTED);
    }
    
    else 
    {
      contoursIter->setState(VISIBLE);
    }

    ++contoursIter;
  }
}


void DialogListContours::setValueToDlgItem(int dlgItem, int value)
{
  CString str;
  str.Format("%d", value);
  SetDlgItemText(dlgItem, str);
}

int DialogListContours::getIntFromDlgItem(int dlgItem)
{
  CString str;
  GetDlgItemText(dlgItem, str);
  return atoi(str);
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
