#pragma once

bool os_higher_than_xp();

std::vector<CString> select_path_dlg(HWND hwndOwner = NULL);

std::vector<CString> get_files_in_path(CString path);

CString change_word_case(CString str, int wcase);

CString YYMMDD(const CTime& t);
CString hhmmss(const CTime& t);

class UpdateDataWnd
{
public:
	UpdateDataWnd(CWnd* pWnd) {
		mpWnd = pWnd;
		mpWnd->UpdateData(TRUE);
	}
	~UpdateDataWnd() {
		mpWnd->UpdateData(FALSE);
	}
private:
	CWnd* mpWnd;
};

template<typename T>
void append(const T& v, std::vector<T>& ary)
{
	ary.push_back(v);
}

template<typename T>
void append(const std::vector<T>& v, std::vector<T>& ary)
{
	std::copy(v.begin(), v.end(), back_inserter(ary));
}