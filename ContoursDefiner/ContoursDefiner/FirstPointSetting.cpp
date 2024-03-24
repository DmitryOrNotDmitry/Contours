#include "FirstPointSetting.h"

IMPLEMENT_DYNAMIC(FirstPointSetting, CDialog)

BEGIN_MESSAGE_MAP(FirstPointSetting, CDialog)
  ON_BN_CLICKED(IDOK, &FirstPointSetting::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &FirstPointSetting::OnBnClickedCancel)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTcontours_table, &FirstPointSetting::OnLvnItemchangedChanlist)
END_MESSAGE_MAP()

FirstPointSetting::FirstPointSetting(CWnd* pParent /*=NULL*/)
  : CDialog(FirstPointSetting::IDD, pParent)
{
}

FirstPointSetting::~FirstPointSetting()
{
}

void FirstPointSetting::OnBnClickedOk()
{
  CDialog::OnOK();
  this->DestroyWindow();
}

void FirstPointSetting::OnBnClickedCancel()
{
  this->DestroyWindow();
}

void FirstPointSetting::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LISTcontours_table, contoursTable);

}

BOOL FirstPointSetting::OnInitDialog()
{
  CDialog::OnInitDialog();

  this->contoursTable.SetExtendedStyle(this->contoursTable.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

  LVCOLUMN column; //Структура для столбцов

  //Первый столбец - для чекбоксов
  column.fmt = LVCFMT_CENTER;
  column.cx = 20;
  column.iOrder = 0;
  column.mask = LVCF_FMT | LVCF_WIDTH;
  this->contoursTable.InsertColumn(0, &column);


  addColumn(LVCFMT_LEFT, 170, "Контур", 1);

  //for (int i = 0; i < 3; i++)
  //{
  //  this->contoursTable.InsertItem(i, ""); //Добавляем строку
  //  this->contoursTable.SetItemText(i, 1, "qweqwe"); //Название канала
  //  this->contoursTable.SetCheck(i, true); //По-умолчанию делаем все каналы
  //}

  return TRUE;
}

void FirstPointSetting::addColumn(int fmt, int width, char* capture, int numCol)
{
  LVCOLUMN column; //Структура для столбцов
  column.fmt = fmt;
  column.cx = width;
  column.pszText = capture;
  column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
  this->contoursTable.InsertColumn(numCol, &column);
}


void FirstPointSetting::addRow(int rowNum, CString name)
{
  this->contoursTable.InsertItem(rowNum, ""); //Добавляем строку
  this->contoursTable.SetItemText(rowNum, 1, name); //Название канала
  this->contoursTable.SetCheck(rowNum, true); //По-умолчанию делаем все каналы
}

std::vector<ContourState> FirstPointSetting::getContoursStates() const
{
  int numRows = contoursTable.GetItemCount();
  std::vector<ContourState> result(numRows, ContourState::VISIBLE);

  for (int i = 0; i < numRows; i++)
  {
    if (!contoursTable.GetCheck(i))
    {
      result[i] = ContourState::HIDDEN;
      continue;
    }

    if (isRowSelected(i))
    {
      result[i] = ContourState::SELECTED;
      continue;
    }
    
  }

  return result;
}



void FirstPointSetting::setValueToDlgItem(int dlgItem, int value)
{
  CString str;
  str.Format("%d", value);
  SetDlgItemText(dlgItem, str);
}

int FirstPointSetting::getIntFromDlgItem(int dlgItem)
{
  CString str;
  GetDlgItemText(dlgItem, str);
  return atoi(str);
}


void FirstPointSetting::OnLvnItemchangedChanlist(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  *pResult = 0;

  RecalcImageViews(hImage);
}

bool FirstPointSetting::isRowSelected(int row) const
{
  UINT nState = contoursTable.GetItemState(row, LVIS_SELECTED);

  if (nState & LVIS_SELECTED)
  {
    return true;
  }

  return false;
}
