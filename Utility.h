#pragma once

using namespace std;

template<typename T>
void append(const T & v, vector<T> & ary);

template<typename T>
void append(const vector<T> & v, vector<T> & ary);

bool os_higher_than_xp();

std::vector<CString> select_path_dlg(HWND hwndOwner = NULL);

std::vector<CString> get_files_in_path(CString path);

template<typename T>
void append(const T & v, vector<T> & ary)
{
	ary.push_back(v);
}

template<typename T>
void append(const vector<T>& v, vector<T>& ary)
{
	std::copy(v.begin(), v.end(), back_inserter(ary));
}

class UpdateDataWnd
{
public:
	UpdateDataWnd(CWnd * pWnd) {
		mpWnd = pWnd;
		mpWnd->UpdateData(TRUE);
	}
	~UpdateDataWnd() {
		mpWnd->UpdateData(FALSE);
	}
private:
	CWnd * mpWnd;
};

CString YYMMDD(CTime t);
CString hhmmss(CTime t);

CString change_word_case(CString str, int wcase);