#include "FirstPointSetting.h"

IMPLEMENT_DYNAMIC(FirstPointSetting, CDialog)

BEGIN_MESSAGE_MAP(FirstPointSetting, CDialog)
  ON_BN_CLICKED(IDOK, &FirstPointSetting::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &FirstPointSetting::OnBnClickedCancel)
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

  LVCOLUMN column; //��������� ��� ��������

  //������ ������� - ��� ���������
  column.fmt = LVCFMT_CENTER;
  column.cx = 20;
  column.iOrder = 0;
  column.mask = LVCF_FMT | LVCF_WIDTH;
  this->contoursTable.InsertColumn(0, &column);


  addColumn(LVCFMT_LEFT, 170, "������", 1);

  //for (int i = 0; i < 3; i++)
  //{
  //  this->contoursTable.InsertItem(i, ""); //��������� ������
  //  this->contoursTable.SetItemText(i, 1, "qweqwe"); //�������� ������
  //  this->contoursTable.SetCheck(i, true); //��-��������� ������ ��� ������
  //}

  return TRUE;
}

void FirstPointSetting::addColumn(int fmt, int width, char* capture, int numCol)
{
  LVCOLUMN column; //��������� ��� ��������
  column.fmt = fmt;
  column.cx = width;
  column.pszText = capture;
  column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
  this->contoursTable.InsertColumn(numCol, &column);
}


void FirstPointSetting::addRow(int rowNum, CString name)
{
  this->contoursTable.InsertItem(rowNum, ""); //��������� ������
  this->contoursTable.SetItemText(rowNum, 1, name); //�������� ������
  this->contoursTable.SetCheck(rowNum, true); //��-��������� ������ ��� ������
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


