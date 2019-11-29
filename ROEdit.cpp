#include "stdafx.h"
#include "ROEdit.h"

IMPLEMENT_DYNAMIC(CROEdit, CEdit)

CROEdit::CROEdit()
{
}

CROEdit::~CROEdit()
{
}

BEGIN_MESSAGE_MAP(CROEdit, CEdit)
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CROEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_END_SELECT_TEXT, 8, 9);

	CEdit::OnLButtonUp(nFlags, point);
}

HBRUSH CROEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor != CTLCOLOR_EDIT) {
		COLORREF clr = RGB(0, 0, 0);
		pDC->SetTextColor(clr);
		clr = RGB(255, 255, 255);
		pDC->SetBkColor(clr);
		HBRUSH br = ::CreateSolidBrush(clr);
		return br;
	}
	// TODO:  如果不应调用父级的处理程序，则返回非 null 画笔
	return NULL;
}

void CROEdit::OnKillFocus(CWnd* pNewWnd)
{
	//CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码
}

BOOL CROEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYUP) {
		//BOOL bCtrl = ::GetKeyState(VK_SHIFT) & 0x80;
		WPARAM  k = pMsg->wParam;
		BOOL b = (k == VK_HOME) ||
			(k == VK_END) ||
			(k == VK_LEFT) ||
			(k == VK_RIGHT) ||
			(k == VK_UP) ||
			(k == VK_DOWN);

		if (b) {
			GetParent()->SendMessage(WM_END_SELECT_TEXT, 8, 9);
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}