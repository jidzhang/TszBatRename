// TszRenameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TszRename.h"
#include "TszRenameDlg.h"
#include "afxdialogex.h"
#include "FileMetaData.h"
#include "resource.h"
#include "SelectTextRangeDlg.h"

using namespace std;
using namespace String;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
//
//class CAboutDlg : public CDialogEx
//{
//public:
//	CAboutDlg();
//
//	// 对话框数据
//#ifdef AFX_DESIGN_TIME
//	enum { IDD = IDD_ABOUTBOX };
//#endif
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//
//														// 实现
//protected:
//	DECLARE_MESSAGE_MAP()
//};
//
//CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
//{
//}
//
//void CAboutDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialogEx::DoDataExchange(pDX);
//}
//
//BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//END_MESSAGE_MAP()

// CTszRenameDlg 对话框

IMPLEMENT_DYNAMIC(CTszRenameDlg, CDialogEx)

CTszRenameDlg::CTszRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TSZRENAME_DIALOG, pParent)
	, m_counter_start(1)
	, m_counter_step(1)
	, m_counter_max_len(0)
	, m_replace_word_case(0)
	, m_file_name_pat(_T("<N>"))
	, m_ext_name_pat(_T("<E>"))
	, m_sort_column(0)
	, m_sort_inc(true)
	, m_renaming(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTszRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COUNTER_START, m_counter_start);
	DDX_Text(pDX, IDC_EDIT_COUNTER_STEP, m_counter_step);
	DDX_CBIndex(pDX, IDC_COMBO_COUNTER_LEN, m_counter_max_len);
	DDV_MinMaxInt(pDX, m_counter_max_len, 0, 9);
	DDX_CBIndex(pDX, IDC_COMBO_WORD_CASE, m_replace_word_case);
	DDX_CBString(pDX, IDC_COMBO_FILENAME_PAT, m_file_name_pat);
	DDX_Control(pDX, IDC_COMBO_OLD_STRING, m_cmb_str_find);
	DDX_Control(pDX, IDC_COMBO_NEW_STRING, m_cmb_str_replace);
	DDX_Text(pDX, IDC_EDIT_EXT_NAME_PAT, m_ext_name_pat);
	DDX_Control(pDX, IDC_LIST_FILE, m_file_list_ctrl);
}

void CTszRenameDlg::InitFileListCtrl()
{
	DWORD style = m_file_list_ctrl.GetExtendedStyle();
	style |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES;
	m_file_list_ctrl.SetExtendedStyle(style);
	CRect rect;
	m_file_list_ctrl.GetWindowRect(&rect);
	int w = rect.right - rect.left;
	m_file_list_ctrl.InsertColumn(0, _T("↑旧文件名"), LVCFMT_LEFT, int(0.25*w));
	m_file_list_ctrl.InsertColumn(1, _T("扩展名"), LVCFMT_LEFT, int(0.10*w));
	m_file_list_ctrl.InsertColumn(2, _T("新文件名"), LVCFMT_LEFT, int(0.25*w));
	m_file_list_ctrl.InsertColumn(3, _T("大小"), LVCFMT_LEFT, int(0.10*w));
	m_file_list_ctrl.InsertColumn(4, _T("日期"), LVCFMT_LEFT, int(0.20*w));
	m_file_list_ctrl.InsertColumn(5, _T("位置"), LVCFMT_LEFT, int(0.30*w));
}

void CTszRenameDlg::AddToolTips()
{
	EnableToolTips(TRUE);
	m_tooltip_ctrl.Create(this);
	m_tooltip_ctrl.Activate(TRUE);
	SetToolTips(IDC_BUTTON_UNDO, _T("撤销重命名"));
	SetToolTips(IDC_CHECK1, _T("区分大小写"));
	SetToolTips(IDC_CHECK2, _T("每个文件只替换一次"));
	SetToolTips(IDC_CHECK3, _T("同时替换文件扩展名"));
	SetToolTips(IDC_COMBO_OLD_STRING, _T("支持正则表达式，非正则式下支持或字符|及通配符*和?"));
	SetToolTips(IDC_COMBO_NEW_STRING, _T("支持正则表达式，非正则式下支持或字符|及通配符*和?"));
}

void CTszRenameDlg::SetToolTips(UINT uId, CString tips)
{
	CWnd* pW = GetDlgItem(uId);
	m_tooltip_ctrl.AddTool(pW, tips);
}

void CTszRenameDlg::AppendToFileList(CString filePath)
{
	if (m_file_meta_data_map.find(filePath) != m_file_meta_data_map.end())
		return;
	CFileMetaData meta(filePath);
	if (!meta.Status())
		return;
	m_file_meta_data_map[filePath] = meta;

	int n = m_file_list_ctrl.GetItemCount();
	int row = m_file_list_ctrl.InsertItem(n, meta.m_file_name);
	meta.m_sort_idx = n + 1;
	m_file_list_ctrl.SetItemText(row, 1, meta.m_file_ext);
	m_file_list_ctrl.SetItemText(row, 2, _T(""));
	m_file_list_ctrl.SetItemText(row, 3, meta.m_file_size);
	m_file_list_ctrl.SetItemText(row, 4, meta.m_file_time);
	m_file_list_ctrl.SetItemText(row, 5, meta.m_file_dir);
	m_file_list_ctrl.SetCheck(row, TRUE);
}

BEGIN_MESSAGE_MAP(CTszRenameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTszRenameDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON10, &CTszRenameDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CTszRenameDlg::OnBnClickedButton_SelectFold)
	ON_BN_CLICKED(IDC_BUTTON12, &CTszRenameDlg::OnBnClickedButton_CleanFiles)
	ON_BN_CLICKED(IDC_BUTTON13, &CTszRenameDlg::OnBnClickedButton_RemoveFiles)
	ON_BN_CLICKED(IDC_BUTTON14, &CTszRenameDlg::OnBnClickedButton_Rename_UNDO)
	ON_BN_CLICKED(IDC_BUTTON2, &CTszRenameDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTszRenameDlg::OnBnClickedButton_Name_Range)
	ON_BN_CLICKED(IDC_BUTTON4, &CTszRenameDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTszRenameDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTszRenameDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CTszRenameDlg::OnBnClickedButton_RenameNow)
	ON_BN_CLICKED(IDC_BUTTON8, &CTszRenameDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CTszRenameDlg::OnBnClickedButton_SelectFiles)
	ON_BN_CLICKED(IDC_CHECK1, &CTszRenameDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CTszRenameDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CTszRenameDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CTszRenameDlg::OnBnClickedCheck4)
	ON_CBN_EDITCHANGE(IDC_COMBO_FILENAME_PAT, &CTszRenameDlg::OnCbnEditchangeComboFilenamePat)
	ON_CBN_EDITCHANGE(IDC_COMBO_NEW_STRING, &CTszRenameDlg::OnCbnEditchangeComboNewString)
	ON_CBN_EDITCHANGE(IDC_COMBO_OLD_STRING, &CTszRenameDlg::OnCbnEditchangeComboOldString)
	ON_CBN_SELCHANGE(IDC_COMBO_COUNTER_LEN, &CTszRenameDlg::OnCbnSelchangeComboCounterLen)
	ON_CBN_SELCHANGE(IDC_COMBO_NEW_STRING, &CTszRenameDlg::OnCbnSelchangeComboNewString)
	ON_CBN_SELCHANGE(IDC_COMBO_OLD_STRING, &CTszRenameDlg::OnCbnSelchangeComboOldString)
	ON_CBN_SELCHANGE(IDC_COMBO_WORD_CASE, &CTszRenameDlg::OnCbnSelchangeComboWordCase)
	ON_EN_CHANGE(IDC_EDIT_COUNTER_START, &CTszRenameDlg::OnEnChangeEditCounterStart)
	ON_EN_CHANGE(IDC_EDIT_COUNTER_STEP, &CTszRenameDlg::OnEnChangeEditCounterStep)
	ON_EN_CHANGE(IDC_EDIT_EXT_NAME_PAT, &CTszRenameDlg::OnEnChangeEditExtNamePat)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FILE, &CTszRenameDlg::OnLvnColumnclickListFile)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILE, &CTszRenameDlg::OnLvnItemchangedListFile)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTszRenameDlg::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTszRenameDlg::OnDeltaposSpin2)
	ON_WM_DROPFILES()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

// CTszRenameDlg 消息处理程序

BOOL CTszRenameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	DragAcceptFiles(TRUE);

	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);
	InitFileListCtrl();
	AddToolTips();

	//
	for (size_t i = 0; i < m_input_file_list.size(); ++i) {
		AppendToFileList(m_input_file_list[i]);
	}
	rename_file_list();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CTszRenameDlg::OnPaint()
{
	if (IsIconic()) {
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	} else {
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTszRenameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int  comare_method = 0;
bool compare_reverse = false;
bool PathLess(CString p1, CString p2)
{
	p1 = p1.MakeLower();
	p2 = p2.MakeLower();
	return StrCmpLogicalW(p1, p2) < 0;
}

bool FileMetaLessFunc(const CFileMetaData & meta1, const CFileMetaData & meta2)
{
	if (meta1.m_checked && !meta2.m_checked)
		return true;
	if (!meta1.m_checked && meta2.m_checked)
		return false;
	if (!meta1.m_checked && !meta2.m_checked) {
		return PathLess(meta1.m_file_name, meta2.m_file_name);
	}
	bool b = false;
	if (comare_method == 0) {
		int bn = meta1.m_file_name.CompareNoCase(meta2.m_file_name);
		if (bn != 0) {
			b = bn < 0;
		} else {
			return false;
		}
		b = PathLess(meta1.m_file_name, meta2.m_file_name);
	} else if (comare_method == 1) {
		int bn = meta1.m_file_ext.CompareNoCase(meta2.m_file_ext);
		if (bn != 0) {
			b = bn < 0;
		} else {
			return false;
		}
		b = PathLess(meta1.m_file_ext, meta2.m_file_ext);
	} else if (comare_method == 3) {
		int n1 = ToInt(ToString(meta1.m_file_size));
		int n2 = ToInt(ToString(meta2.m_file_size));
		int bn = n1 - n2;
		if (bn != 0) {
			b = bn < 0;
		} else {
			return false;
		}
	} else if (comare_method == 4) {
		int bn = meta1.m_file_time.CompareNoCase(meta2.m_file_time);
		if (bn != 0) {
			b = bn < 0;
		} else {
			return false;
		}
	} else if (comare_method == 5) {
		int bn = meta1.m_file_dir.CompareNoCase(meta2.m_file_dir);
		if (bn != 0) {
			b = bn < 0;
		} else {
			return false;
		}
		b = PathLess(meta1.m_file_dir, meta2.m_file_dir);
	}

	return compare_reverse ? !b : b;
}

bool FileMetaLessFunc2(const CFileMetaData & meta1, const CFileMetaData & meta2)
{
	return meta1.m_sort_idx < meta2.m_sort_idx;
}

void CTszRenameDlg::set_file_list(vector<CString> input_file_list)
{
	m_input_file_list.clear();
	for (size_t i = 0; i < input_file_list.size(); ++i) {
		CString filePath = input_file_list[i];
		if (filePath.IsEmpty())
			continue;
		if (filePath[0] == _T('"')) {
			filePath = filePath.Right(filePath.GetLength() - 1);
		}
		if (filePath.IsEmpty())
			continue;
		if (filePath[filePath.GetLength() - 1] == _T('"')) {
			filePath = filePath.Left(filePath.GetLength() - 1);
		}
		if (filePath.IsEmpty())
			continue;
		m_input_file_list.push_back(filePath);
	}
}

void CTszRenameDlg::sort_file_list()
{
	int n = m_file_list_ctrl.GetItemCount();
	if (n == 0)
		return;
	for (int i = 0; i < n; ++i) {
		CString name = m_file_list_ctrl.GetItemText(i, 0);
		CString ext = m_file_list_ctrl.GetItemText(i, 1);
		CString path = m_file_list_ctrl.GetItemText(i, 5);
		CString fullPath = path + name + _T(".") + ext;
		bool b = m_file_list_ctrl.GetCheck(i);
		auto itr = m_file_meta_data_map.find(fullPath);
		if (itr != m_file_meta_data_map.end()) {
			itr->second.m_checked = b;
		}
	}
	vector<CFileMetaData> file_list;
	auto itr = m_file_meta_data_map.begin();
	for (; itr != m_file_meta_data_map.end(); ++itr) {
		file_list.push_back(itr->second);
	}
	std::sort(file_list.begin(), file_list.end(), FileMetaLessFunc);
	for (size_t i = 0; i < file_list.size(); ++i) {
		file_list[i].m_sort_idx = i + 1;
		m_file_meta_data_map[file_list[i].m_file_path].m_sort_idx = i + 1;
	}
}

void CTszRenameDlg::rename_file_list()
{
	if (!m_renaming) {
		m_file_idx_to_name_map.clear();
		m_renaming = true;
		sort_file_list();
		vector<CString> name_pats = split_name_pattern(m_file_name_pat);
		vector<CString> ext_pats = split_name_pattern(m_ext_name_pat);
		auto itr = m_file_meta_data_map.begin();
		for (; itr != m_file_meta_data_map.end(); ++itr) {
			CFileMetaData & meta = itr->second;
			CString name = replace_str_by_pat(meta, name_pats, false);
			CString ext = replace_str_by_pat(meta, ext_pats, true);
			meta.m_file_new = name + _T(".") + ext;
			int idx = meta.m_sort_idx - 1;
			m_file_list_ctrl.SetItemText(idx, 0, meta.m_file_name);
			m_file_list_ctrl.SetItemText(idx, 1, meta.m_file_ext);
			m_file_list_ctrl.SetItemText(idx, 2, meta.m_file_new);
			m_file_list_ctrl.SetItemText(idx, 3, meta.m_file_size);
			m_file_list_ctrl.SetItemText(idx, 4, meta.m_file_time);
			m_file_list_ctrl.SetItemText(idx, 5, meta.m_file_dir);
			m_file_list_ctrl.SetCheck(idx, meta.m_checked);
			m_file_idx_to_name_map[idx] = itr->first;
		}
		replace_str_by_pat();
		m_renaming = false;
	}
}

CString formatNumber(int no, int length)
{
	CString ans;
	ans.Format(_T("%d"), no);
	const int len = ans.GetLength();
	for (int i = 0; i < length - len; ++i) {
		ans = _T("0") + ans;
	}
	return ans;
}

CString CTszRenameDlg::replace_str_by_pat(const CFileMetaData & meta, vector<CString> pats, bool ext /*= false*/)
{
	CTime now = CTime::GetCurrentTime();
	CString ymd = YYMMDD(now);
	CString hms = hhmmss(now);

	CString tmp_str = ext ? meta.m_file_ext : meta.m_file_name;
	if (!meta.m_checked)
		return tmp_str;
	CString ans = _T("");
	for (size_t i = 0; i < pats.size(); ++i) {
		CString pat = pats[i];
		if (pat == _T("<N>") || pat == _T("<E>")) {
			ans += tmp_str;
		} else if (pat == _T("<C>")) {
			int idx = meta.m_sort_idx;
			idx = m_counter_start + (idx - 1) * m_counter_step;
			//ans.Format(_T("%s%d"), ans, idx);
			ans += formatNumber(idx, m_counter_max_len + 1);
		} else if (pat == _T("<YMD>")) {
			ans += ymd;
		} else if (pat == _T("<hms>")) {
			ans += hms;
		} else if (pat[0] != '<') {
			ans += pat;
		} else if (pat.Find(_T("<N")) == 0 || pat.Find(_T("<E")) == 0) {
			pat = pat.Right(pat.GetLength() - 2);
			if (pat.Find(_T(">")) > 0) {
				pat = pat.Left(pat.GetLength() - 1);
				int nS, nE, len;
				if (!pat.IsEmpty()) {
					nS = _tstoi(pat.GetBuffer());
					pat.ReleaseBuffer();
					if (nS < 0) {
						nS = tmp_str.GetLength() + nS + 1;
					}
					if (pat[0] == _T('-')) {
						pat = pat.Right(pat.GetLength() - 1);
					}
					int n = pat.Find(_T(","));
					if (n > 0) {
						pat = pat.Right(pat.GetLength() - n - 1);
						len = _tstoi(pat.GetBuffer());
						pat.ReleaseBuffer();
					} else {
						n = pat.Find(_T("-"));
						if (n > 0) {
							pat = pat.Right(pat.GetLength() - n - 1);
							nE = _tstoi(pat.GetBuffer());
							pat.ReleaseBuffer();
							if (nE < 0) {
								nE = tmp_str.GetLength() + nE + 1;
							}
							len = nE - nS + 1;
						} else {
							len = 1;
						}
					}
				}
				if (len < 0)
					len = 0;
				if (len) {
					ans += tmp_str.Mid(nS - 1, len);
				}
			}
		}
	}
	if (ans.IsEmpty()) {
		return ans;
	}
	if (ext && m_replace_word_case > 2) {
	} else {
		ans = change_word_case(ans, m_replace_word_case);
	}
	return ans;
}

std::vector<CString> CTszRenameDlg::split_name_pattern(CString name_pat)
{
	std::vector<CString> ans;
	CString pat;
	for (int i = 0; i < name_pat.GetLength(); ++i) {
		TCHAR c = name_pat[i];
		if (c == L'<') {
			if (!pat.IsEmpty()) {
				ans.push_back(pat);
				pat = _T("");
			}
			pat += c;
			while (i < name_pat.GetLength()) {
				++i;
				c = name_pat[i];
				if (c == L'>') {
					pat += c;
					ans.push_back(pat);
					pat = _T("");
					break;
				} else {
					pat += c;
				}
			}
		} else {
			pat += c;
		}
	}
	if (!pat.IsEmpty()) {
		ans.push_back(pat);
		pat = _T("");
	}
	return ans;
}

void CTszRenameDlg::set_word_case()
{
	if (m_replace_word_case == 0)
		return;
}

bool StringSearch(CString src, CString pat, int & begin, int & end)
{
	int p = pat.Find(_T("**"));
	while (p >= 0) {
		pat.Delete(p, 1);
		p = pat.Find(_T("**"));
	}
	p = pat.Find(_T("*"));
	if (p == 0) {
		pat = pat.Right(pat.GetLength() - 1);
	}
	if (begin < 0)
		begin = 0;
	if (end < 0)
		end = 0;
	int sidx = begin;
	while (sidx < src.GetLength()) {
		bool b = StringMatch(src, pat, sidx, end);
		if (b) {
			begin = sidx;
			return true;
		}
		++sidx;
	}
	return false;
}

static bool g_cmp_ignore_case = true;

bool StringMatch(CString src, CString pat, int begin, int & end)
{
	int p = pat.Find(_T("**"));
	while (p >= 0) {
		pat.Delete(p, 1);
		p = pat.Find(_T("**"));
	}
	if (begin < 0)
		begin = 0;
	if (end < 0)
		end = 0;
	int sidx = begin, pidx = 0;
	while (sidx < src.GetLength() && pidx < pat.GetLength()) {
		if (_T('*') == pat[pidx]) {
			CString patNext = pat.Right(pat.GetLength() - pidx - 1);
			bool b = StringSearch(src, patNext, sidx, end);
			return b;
		}
		bool b = g_cmp_ignore_case ? (_tolower(src[sidx]) == _tolower(pat[pidx])) : (src[sidx] == pat[pidx]);
		if (_T('?') == pat[pidx] || b) {
			++sidx;
			++pidx;
			continue;
		}
		return false;
	}
	if (pidx >= pat.GetLength()) {
		end = sidx;
		return true;
	}
	return false;
}

void CTszRenameDlg::replace_str_by_pat()
{
	BOOL chk1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	BOOL chk2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	BOOL chk3 = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	BOOL chk4 = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();
	CString findStr, replaceStr;
	GetDlgItemText(IDC_COMBO_OLD_STRING, findStr);
	GetDlgItemText(IDC_COMBO_NEW_STRING, replaceStr);
	if (findStr.IsEmpty())
		return;

	g_cmp_ignore_case = chk1 == FALSE;

	if (!chk4) {	//非正则
		vector<CString> findGroups;
		vector<CString> replaceGroups;
		CString item = _T("");
		for (int i = 0; i < findStr.GetLength(); ++i) {
			TCHAR c = findStr[i];
			if (c == _T('|')) {
				if (!item.IsEmpty()) {
					findGroups.push_back(item);
					item = _T("");
				}
			} else {
				item += c;
			}
		}
		if (!item.IsEmpty()) {
			findGroups.push_back(item);
			item = _T("");
		}
		for (int i = 0; i < replaceStr.GetLength(); ++i) {
			TCHAR c = replaceStr[i];
			if (c == _T('|')) {
				if (!item.IsEmpty()) {
					replaceGroups.push_back(item);
					item = _T("");
				}
			} else {
				item += c;
			}
		}
		if (!item.IsEmpty()) {
			replaceGroups.push_back(item);
			item = _T("");
		}
		for (size_t i = 0; i < findGroups.size(); ++i) {
			CString pat = findGroups[i];
			CString replace = _T("");
			if (replaceGroups.size() == 1) {
				replace = replaceGroups[0];
			} else if (i < replaceGroups.size()) {
				replace = replaceGroups[i];
			}
			int n = m_file_list_ctrl.GetItemCount();
			for (int j = 0; j < n; ++j) {
				CString oldName = m_file_idx_to_name_map[j];
				CString str = m_file_meta_data_map[oldName].m_file_new;
				CString ext = _T("");
				//文件后缀不替换
				if (!chk3) {
					int n = str.ReverseFind(_T('.'));
					if (n >= 0) {
						ext = str.Right(str.GetLength() - n);
						str = str.Left(n);
					}
				}
				CString tmp = _T("");
				int begin = 0;
				int end = 0;
				int last = 0;
				bool b = StringSearch(str, pat, begin, end);
				while (b) {
					tmp += str.Mid(last, begin - last);
					tmp += (replace == _T("<清除>") || replace == _T("")) ? _T("") : replace;
					if (chk2)
						break;
					begin = end;
					last = end;
					b = StringSearch(str, pat, begin, end);
				}
				tmp += str.Right(str.GetLength() - end) + ext;
				m_file_meta_data_map[oldName].m_file_new = tmp;
				m_file_list_ctrl.SetItemText(j, 2, tmp);
			}
		}
	} else {
		//正则表达式
		auto p = ToString(findStr);
		auto r = ToString(replaceStr);
		if (r == "<清除>")
			r = "";
		try {
			regex rgx(p, regex::icase);
			if (chk1) {
				rgx.assign(p);
			}
			int n = m_file_list_ctrl.GetItemCount();
			for (int j = 0; j < n; ++j) {
				CString oldName = m_file_idx_to_name_map[j];
				CString str = m_file_meta_data_map[oldName].m_file_new;
				CString ext = _T("");
				//文件后缀不替换
				if (!chk3) {
					int n = str.ReverseFind(_T('.'));
					if (n >= 0) {
						ext = str.Right(str.GetLength() - n);
						str = str.Left(n);
					}
				}
				auto ss = ToString(str);
				if (chk2) {
					ss = regex_replace(ss, rgx, r, regex_constants::format_first_only);
				} else {
					ss = regex_replace(ss, rgx, r);
				}
				str = ToCString(ss) + ext;
				m_file_meta_data_map[oldName].m_file_new = str;
				m_file_list_ctrl.SetItemText(j, 2, str);
			}
		} catch (...) {
		}
	}
}

void CTszRenameDlg::OnBnClickedButton_SelectFiles()
{
	UpdateDataWnd upd(this);
	CFileDialog fdlg(TRUE, _T("*.*"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT,
		_T("所有文件(*.*)|*.*|PDF文件(*.pdf)|*.pdf|图纸(*.dwg;*.dwf)|*.dwg;*.dwf||"));
	const DWORD numberOfFileNames = 100;
	const DWORD fileNameMaxLength = MAX_PATH + 1;
	const DWORD bufferSize = numberOfFileNames * fileNameMaxLength + 1;
	TCHAR * buffer = new TCHAR[bufferSize];
	buffer[0] = NULL;
	buffer[bufferSize - 1] = NULL;
	fdlg.m_ofn.lpstrFile = buffer;
	fdlg.m_ofn.nMaxFile = bufferSize;

	if (fdlg.DoModal() != IDOK) {
		delete[] buffer;
		return;
	}

	POSITION p = fdlg.GetStartPosition();
	while (p) {
		CString str = fdlg.GetNextPathName(p);
		AppendToFileList(str);
	}
	delete[] buffer;

	rename_file_list();
}

void CTszRenameDlg::OnBnClickedButton_SelectFold()
{
	static bool selecting = false;
	if (!selecting) {
		selecting = true;
		vector<CString> folders = select_path_dlg(GetSafeHwnd());
		for (size_t i = 0; i < folders.size(); ++i) {
			CString path = folders[i];
			if (!path.IsEmpty()) {
				vector<CString> files = get_files_in_path(path);
				for (size_t j = 0; j < files.size(); ++j) {
					AppendToFileList(files[j]);
				}
			}
		}
		selecting = false;
	}
}

void CTszRenameDlg::OnBnClickedButton_CleanFiles()
{
	m_file_list_ctrl.DeleteAllItems();
	m_file_meta_data_map.clear();
}

void CTszRenameDlg::OnBnClickedButton_RemoveFiles()
{
	set<int> selected_items;
	POSITION pos = m_file_list_ctrl.GetFirstSelectedItemPosition();
	while (pos) {
		int n = m_file_list_ctrl.GetNextSelectedItem(pos);
		if (n >= 0) {
			selected_items.insert(n);
		}
	}
	auto itr = selected_items.rbegin();
	for (; itr != selected_items.rend(); ++itr) {
		int idx = *itr;
		CString name = m_file_list_ctrl.GetItemText(idx, 0);
		CString ext = m_file_list_ctrl.GetItemText(idx, 1);
		CString dir = m_file_list_ctrl.GetItemText(idx, 5);
		CString path = dir + _T("\\") + name + _T(".") + ext;
		m_file_meta_data_map.erase(path);
		m_file_list_ctrl.DeleteItem(idx);
	}
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedButton1()
{
	UpdateDataWnd upd(this);
	m_file_name_pat += _T("<N>");
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedButton2()
{
	UpdateDataWnd upd(this);
	m_file_name_pat += _T("<YMD>");
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedButton4()
{
	UpdateDataWnd upd(this);
	m_file_name_pat += _T("<hms>");
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedButton5()
{
	UpdateDataWnd upd(this);
	m_file_name_pat += _T("<C>");
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedButton6()
{
	UpdateDataWnd upd(this);
	m_ext_name_pat += _T("<E>");
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedButton10()
{
	UpdateDataWnd upd(this);
	m_ext_name_pat += _T("<C>");
	rename_file_list();
}

void CTszRenameDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateDataWnd upd(this);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMUpDown->iDelta < 0)
		m_counter_start += 1;
	else
		m_counter_start -= 1;
	rename_file_list();
}

void CTszRenameDlg::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateDataWnd upd(this);
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMUpDown->iDelta < 0)
		m_counter_step += 1;
	else
		m_counter_step -= 1;
	rename_file_list();
}

void CTszRenameDlg::OnCbnEditchangeComboFilenamePat()
{
	UpdateData(TRUE);
	rename_file_list();
}

void CTszRenameDlg::OnCbnSelchangeComboWordCase()
{
	UpdateDataWnd upd(this);
	rename_file_list();
}

void CTszRenameDlg::OnEnChangeEditExtNamePat()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	rename_file_list();
}

void CTszRenameDlg::OnLvnColumnclickListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int sort_column = pNMLV->iSubItem;
	comare_method = sort_column;
	if (sort_column == 2)
		return;
	TCHAR buffer[256];
	LVCOLUMN  pColumn;
	pColumn.mask = LVCF_TEXT;
	pColumn.pszText = buffer;
	pColumn.cchTextMax = 256;
	memset(buffer, 0, sizeof(TCHAR) * 256);
	m_file_list_ctrl.GetColumn(sort_column, &pColumn);
	CString str = pColumn.pszText;
	if (str.Find(_T("↑")) >= 0) {
		str.SetAt(0, _T('↓'));
		m_sort_inc = false;
		compare_reverse = true;
	} else if (str.Find(_T("↓")) >= 0) {
		str.SetAt(0, _T('↑'));
		m_sort_inc = true;
		compare_reverse = false;
	} else {
		str.Insert(0, _T('↑'));
		m_sort_inc = true;
		compare_reverse = false;
	}
	pColumn.pszText = str.GetBuffer();
	str.ReleaseBuffer();
	m_file_list_ctrl.SetColumn(sort_column, &pColumn);
	for (int i = 0; i < 6; ++i) {
		if (sort_column == i)
			continue;
		LVCOLUMN  pColumn;
		pColumn.mask = LVCF_TEXT;
		pColumn.pszText = buffer;
		pColumn.cchTextMax = 256;
		memset(buffer, 0, sizeof(TCHAR) * 256);
		m_file_list_ctrl.GetColumn(i, &pColumn);
		CString str = pColumn.pszText;
		str.Remove(_T('↑'));
		str.Remove(_T('↓'));
		pColumn.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		m_file_list_ctrl.SetColumn(i, &pColumn);
	}
	rename_file_list();
}

void CTszRenameDlg::OnLvnItemchangedListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	if (pNMLV->uNewState == 0x1000 && pNMLV->uOldState == 0x2000 || pNMLV->uNewState == 0x2000 && pNMLV->uOldState == 0x1000) {
		if (!m_renaming) {
			rename_file_list();
		}
	}
}

void CTszRenameDlg::OnBnClickedButton_RenameNow()
{
	//先检测有没有重名的
	set<CString> tmp_name_set;
	auto itr = m_file_meta_data_map.begin();
	for (; itr != m_file_meta_data_map.end(); ++itr) {
		const CFileMetaData & meta = itr->second;
		if (meta.m_checked) {
			if (tmp_name_set.find(meta.m_file_new) != tmp_name_set.end()) {
				break;
			}
			tmp_name_set.insert(meta.m_file_new);
		}
	}
	if (itr != m_file_meta_data_map.end()) {
		MessageBox(_T("发现重名的文件，是否继续执行？"), _T("批量重命名"));
	}

	m_new_name_map.clear();
	itr = m_file_meta_data_map.begin();
	vector<CFileMetaData> file_list;
	for (; itr != m_file_meta_data_map.end(); ++itr) {
		const CFileMetaData & meta = itr->second;
		if (meta.m_checked) {
			CString str = meta.m_file_dir + meta.m_file_new;
			if (str != meta.m_file_path) {
				file_list.push_back(meta);
				m_new_name_map[str] = meta.m_file_path;
			}
		}
	}
	sort(file_list.begin(), file_list.end(), FileMetaLessFunc2);
	for (size_t i = 0; i < file_list.size(); ++i) {
		const CFileMetaData & meta = file_list[i];
		CString str = meta.m_file_dir + meta.m_file_new + _T(".tmp");
		MoveFile(meta.m_file_path, str);
	}
	int count = 0;
	for (size_t i = 0; i < file_list.size(); ++i) {
		const CFileMetaData & meta = file_list[i];
		CString str1 = meta.m_file_dir + meta.m_file_new + _T(".tmp");
		CString str2 = meta.m_file_dir + meta.m_file_new;
		BOOL b = MoveFile(str1, str2);
		count = b ? count + 1 : count;
	}
	if (count > 0) {
		GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(TRUE);
		CString str;
		str.Format(_T("批量重命名已完成，成功 %d 个！"), count);
		MessageBox(str, _T("批量重命名"));
	}
}

void CTszRenameDlg::OnBnClickedButton_Rename_UNDO()
{
	map<CString, CString>::iterator itr = m_new_name_map.begin();
	for (; itr != m_new_name_map.end(); ++itr) {
		MoveFile(itr->first, itr->first + _T(".tmp"));
	}
	itr = m_new_name_map.begin();
	for (; itr != m_new_name_map.end(); ++itr) {
		MoveFile(itr->first + _T(".tmp"), itr->second);
	}
	m_new_name_map.clear();
	GetDlgItem(IDC_BUTTON_UNDO)->EnableWindow(FALSE);
	CString str;
	str.Format(_T("批量重命名已撤销！"));
	MessageBox(str, _T("批量重命名"));
}

BOOL CTszRenameDlg::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_tooltip_ctrl.GetSafeHwnd()) {
		m_tooltip_ctrl.RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTszRenameDlg::OnBnClickedButton_Name_Range()
{
	UpdateDataWnd(this);
	set<CString> name_set;
	set<CString> ext_set;
	POSITION pos = m_file_list_ctrl.GetFirstSelectedItemPosition();
	while (pos) {
		int n = m_file_list_ctrl.GetNextSelectedItem(pos);
		if (n >= 0) {
			name_set.insert(m_file_list_ctrl.GetItemText(n, 0));
			ext_set.insert(m_file_list_ctrl.GetItemText(n, 1));
		}
	}
	if (name_set.empty()) {
		int n = m_file_list_ctrl.GetItemCount();
		for (int i = 0; i < n; ++i) {
			name_set.insert(m_file_list_ctrl.GetItemText(i, 0));
			ext_set.insert(m_file_list_ctrl.GetItemText(i, 1));
		}
	}
	CString str;
	auto itr = name_set.begin();
	for (; itr != name_set.end(); ++itr) {
		if (itr->GetLength() > str.GetLength())
			str = *itr;
	}
	if (str.IsEmpty()) {
		MessageBox(_T("文件列表为空，无法选择文本范围"), _T("批量重命名"));
	} else {
		CSelectTextRangeDlg dlg;
		dlg.SetOriginText(str);
		int rt = dlg.DoModal();
		if (rt == IDOK) {
			CString str = dlg.GetTextRange();
			if (!str.IsEmpty()) {
				m_file_name_pat += _T("<N") + str + _T(">");
				rename_file_list();
				UpdateData(FALSE);
			}
		}
	}
}

void CTszRenameDlg::OnBnClickedButton8()
{
	UpdateDataWnd(this);
	set<CString> name_set;
	set<CString> ext_set;
	POSITION pos = m_file_list_ctrl.GetFirstSelectedItemPosition();
	while (pos) {
		int n = m_file_list_ctrl.GetNextSelectedItem(pos);
		if (n >= 0) {
			name_set.insert(m_file_list_ctrl.GetItemText(n, 0));
			ext_set.insert(m_file_list_ctrl.GetItemText(n, 1));
		}
	}
	if (name_set.empty()) {
		int n = m_file_list_ctrl.GetItemCount();
		for (int i = 0; i < n; ++i) {
			name_set.insert(m_file_list_ctrl.GetItemText(i, 0));
			ext_set.insert(m_file_list_ctrl.GetItemText(i, 1));
		}
	}
	CString str;
	set<CString>::iterator itr = ext_set.begin();
	for (; itr != ext_set.end(); ++itr) {
		if (itr->GetLength() > str.GetLength())
			str = *itr;
	}
	if (str.IsEmpty()) {
		MessageBox(_T("文件列表为空，无法选择文本范围"), _T("批量重命名"));
	} else {
		CSelectTextRangeDlg dlg;
		dlg.SetOriginText(str);
		int rt = dlg.DoModal();
		if (rt == IDOK) {
			CString str = dlg.GetTextRange();
			if (!str.IsEmpty()) {
				m_ext_name_pat += _T("<E") + str + _T(">");
				rename_file_list();
				UpdateData(FALSE);
			}
		}
	}
}

void CTszRenameDlg::OnCbnSelchangeComboOldString()
{
	UpdateDataWnd(this);
	CString findStr, replaceStr;
	GetDlgItemText(IDC_COMBO_OLD_STRING, findStr);
	GetDlgItemText(IDC_COMBO_NEW_STRING, replaceStr);
	if (replaceStr.IsEmpty()) {
		SetDlgItemText(IDC_COMBO_NEW_STRING, _T("<清除>"));
	}
	rename_file_list();
}

void CTszRenameDlg::OnCbnEditchangeComboOldString()
{
	UpdateDataWnd(this);
	CString findStr, replaceStr;
	GetDlgItemText(IDC_COMBO_OLD_STRING, findStr);
	GetDlgItemText(IDC_COMBO_NEW_STRING, replaceStr);
	if (replaceStr.IsEmpty()) {
		SetDlgItemText(IDC_COMBO_NEW_STRING, _T("<清除>"));
	}
	rename_file_list();
}

void CTszRenameDlg::OnCbnSelchangeComboNewString()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnCbnEditchangeComboNewString()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedCheck1()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedCheck2()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedCheck3()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnBnClickedCheck4()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnEnChangeEditCounterStart()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnEnChangeEditCounterStep()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnCbnSelchangeComboCounterLen()
{
	UpdateDataWnd(this);
	rename_file_list();
}

void CTszRenameDlg::OnDropFiles(HDROP hDropInfo)
{
	int count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (int i = 0; i < count; ++i) {
		TCHAR buffer[MAX_PATH] = { 0 };
		if (DragQueryFile(hDropInfo, i, buffer, MAX_PATH) > 0) {
			AppendToFileList(buffer);
		}
	}
	CDialogEx::OnDropFiles(hDropInfo);
}