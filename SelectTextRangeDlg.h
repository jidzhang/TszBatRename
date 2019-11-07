#pragma once

#include "ROEdit.h"

// CSelectTextRangeDlg 对话框

class CSelectTextRangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectTextRangeDlg)

public:
	CSelectTextRangeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectTextRangeDlg();
	void SetOriginText(CString str);
	CString GetTextRange();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TSZRENAME_TOOL_DIALOG };
#endif
private:
	CROEdit m_edit_ctl;
	CString m_text_range;
	CString m_origin_text;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnEndSelectText(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck1();
};
