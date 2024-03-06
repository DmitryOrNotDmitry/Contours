#include "FirstPointSetting.h"

IMPLEMENT_DYNAMIC(FirstPointSetting, CDialog)

BEGIN_MESSAGE_MAP(FirstPointSetting, CDialog)
  ON_BN_CLICKED(IDOK, &FirstPointSetting::OnBnClickedOk)
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
  startPoint.x = getIntFromDlgItem(IDC_MFCMASKEDEDIT_x);
  startPoint.y = getIntFromDlgItem(IDC_MFCMASKEDEDIT_y);

  CDialog::OnOK();
}

void FirstPointSetting::DoDataExchange(CDataExchange* pDX)
{

  CDialog::DoDataExchange(pDX);
}

BOOL FirstPointSetting::OnInitDialog()
{
  CDialog::OnInitDialog();

  setValueToDlgItem(IDC_MFCMASKEDEDIT_x, startPoint.x);
  setValueToDlgItem(IDC_MFCMASKEDEDIT_y, startPoint.y);

  return TRUE;
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
