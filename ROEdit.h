#pragma once

#define WM_END_SELECT_TEXT WM_USER+106

// CROEdit

class CROEdit : public CEdit
{
	DECLARE_DYNAMIC(CROEdit)

public:
	CROEdit();
	virtual ~CROEdit();

private:
	HBRUSH m_hBrush;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
