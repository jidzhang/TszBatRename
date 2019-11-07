#include "stdafx.h"
#include "SelectTextRangeDlg.h"
#include "afxdialogex.h"

// CSelectTextRangeDlg 对话框

IMPLEMENT_DYNAMIC(CSelectTextRangeDlg, CDialogEx)

CSelectTextRangeDlg::CSelectTextRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TSZRENAME_TOOL_DIALOG, pParent)
	, m_text_range(_T(""))
	, m_origin_text(_T(""))
{
}

CSelectTextRangeDlg::~CSelectTextRangeDlg()
{
}

void CSelectTextRangeDlg::SetOriginText(CString str)
{
	m_origin_text = str;
}

CString CSelectTextRangeDlg::GetTextRange()
{
	return m_text_range;
}

void CSelectTextRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSelectTextRangeDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_END_SELECT_TEXT, OnEndSelectText)
	ON_BN_CLICKED(IDC_CHECK3, &CSelectTextRangeDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK2, &CSelectTextRangeDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK1, &CSelectTextRangeDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()

// CSelectTextRangeDlg 消息处理程序

BOOL CSelectTextRangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemText(IDC_EDIT1, m_origin_text);
	m_edit_ctl.SubclassDlgItem(IDC_EDIT1, this);
	SetDlgItemText(IDC_STATIC_RANGE_TEXT, _T(""));
	//((CEdit*)GetDlgItem(IDC_EDIT1))->SetReadOnly(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

LRESULT CSelectTextRangeDlg::OnEndSelectText(WPARAM wParam, LPARAM lParam)
{
	BOOL b1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	BOOL b2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	BOOL b3 = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	CString str1;
	GetDlgItemText(IDC_EDIT1, str1);
	int sLen = str1.GetLength();
	int nS, nE, len;
	m_edit_ctl.GetSel(nS, nE);
	len = nE - nS;
	nS = b1 ? nS - sLen : nS + 1;
	nE = b2 ? nE - sLen - 1 : nE;
	CString str = _T("");

	if (b3) {
		str.Format(_T("%d,%d"), nS, len);
	} else {
		if (len == 0) {
			str = _T("");
		} else if (len == 1) {
			str.Format(_T("%d"), nS);
		} else {
			str.Format(_T("%d-%d"), nS, nE);
		}
	}

	//
	SetDlgItemText(IDC_STATIC_RANGE_TEXT, str);
	m_text_range = str;
	return 0;
}

void CSelectTextRangeDlg::OnBnClickedCheck3()
{
	BOOL b = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	if (b) {
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(FALSE);
	}
	OnEndSelectText(0, 0);
}

void CSelectTextRangeDlg::OnBnClickedCheck2()
{
	BOOL b = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	if (b) {
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(FALSE);
	}
	OnEndSelectText(0, 0);
}

void CSelectTextRangeDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	OnEndSelectText(0, 0);
}