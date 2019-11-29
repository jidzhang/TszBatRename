#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "FileMetaData.h"

bool PathLess(CString p1, CString p2);
struct PathLessPred : std::binary_function<CString, CString, bool>
{
	bool operator()(CString p1, CString p2) const {
		return PathLess(p1, p2);
	}
};

class CTszRenameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTszRenameDlg)
public:
	CTszRenameDlg(CWnd* pParent = NULL);	// 标准构造函数

	enum { IDD = IDD_TSZRENAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void InitFileListCtrl();
	void AddToolTips();
	void SetToolTips(UINT uId, CString tips);
	void AppendToFileList(CString filePath);
	void InsertStringToNamePat(CString mark);
	void InsertStringToEXTPat(CString mark);

	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CToolTipCtrl m_tooltip_ctrl;
	int m_counter_start;		// 计数器起始值
	int m_counter_step;			// 计数器步长
	int m_counter_max_len;		// 计数器位数
	int m_replace_word_case;	// 文件名大小写规则
	DWORD m_n_file_name_pos;
	CString m_file_name_pat;	// 文件名规则
	CString m_ext_name_pat;		// 扩展名规则
	CComboBox m_cmb_str_find;	// 查找
	CComboBox m_cmb_str_replace;// 替换为
	CListCtrl m_file_list_ctrl;	// 文件列表
	int m_sort_column;			//排序列
	bool m_sort_inc;			//升序
	bool m_renaming;			//正在重命名
	map<int, CString> m_file_idx_to_name_map;
	std::map<CString, CFileMetaData, PathLessPred> m_file_meta_data_map;
	map<CString, CString> m_new_name_map;
	vector<CString> m_input_file_list;

public:
	void set_file_list(vector<CString> input_file_list);
	void sort_file_list();
	void rename_file_list();
	void set_word_case();
	void replace_str_by_pat();
	CString replace_str_by_pat(const CFileMetaData& meta, vector<CString> pats, bool ext = false);
	vector<CString> split_name_pattern(CString name_pat);

	afx_msg void OnBnClickedButton_SelectFiles();
	afx_msg void OnBnClickedButton_SelectFold();
	afx_msg void OnBnClickedButton_CleanFiles();
	afx_msg void OnBnClickedButton_RemoveFiles();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnEditchangeComboFilenamePat();
	afx_msg void OnCbnSelchangeComboWordCase();
	afx_msg void OnEnChangeEditExtNamePat();
	afx_msg void OnLvnColumnclickListFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedListFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton_RenameNow();
	afx_msg void OnBnClickedButton_Rename_UNDO();
	afx_msg void OnBnClickedButton_Name_Range();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnCbnSelchangeComboOldString();
	afx_msg void OnCbnEditchangeComboOldString();
	afx_msg void OnCbnSelchangeComboNewString();
	afx_msg void OnCbnEditchangeComboNewString();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnEnChangeEditCounterStart();
	afx_msg void OnEnChangeEditCounterStep();
	afx_msg void OnCbnSelchangeComboCounterLen();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CComboBox m_cmbFilename;
	CEdit m_edEXT;
};

bool StringSearch(CString src, CString pat, int& begin, int& end);

bool StringMatch(CString src, CString pat, int begin, int& end);
