#include "stdafx.h"
#include "Utility.h"

#pragma warning(disable: 4996)

bool os_higher_than_xp()
{
    bool ans = false;
    OSVERSIONINFO osInfo;
    osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osInfo);
    if (osInfo.dwPlatformId == 2) {
        if (osInfo.dwMajorVersion >= 6)
            ans = true;
    }
    return ans;
}

CString select_path_dlg_xp(HWND hwndOwner)
{
    CString ans;
    BROWSEINFO bi = { 0 };
    memset(&bi, 0, sizeof(bi));
    bi.hwndOwner = hwndOwner;
    bi.lpszTitle = _T("选择一个文件夹");
    bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (!pidl) {
        return ans;
    }
    TCHAR path[MAX_PATH];
    SHGetPathFromIDList(pidl, path);
    ans = path;

    IMalloc * imalloc = 0;
    if (SUCCEEDED(SHGetMalloc(&imalloc))) {
        imalloc->Free(pidl);
        imalloc->Release();
    }
    return ans;
}
std::vector<CString> select_path_dlg(HWND hwndOwner)
{
    std::vector<CString> ans;
    if (os_higher_than_xp()) {
#if _MSC_VER >= 1600
        CWnd * pWnd = hwndOwner != NULL ? CWnd::FromHandle(hwndOwner) : NULL;
        CFolderPickerDialog dlg(NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING, pWnd);
        if (dlg.DoModal() == IDOK) {
            POSITION pos = dlg.GetStartPosition();
            while (pos) {
                ans.push_back(dlg.GetNextPathName(pos));
            }
        }
#else
        ans.push_back(select_path_dlg_xp(hwndOwner));
#endif
    } else {
        ans.push_back(select_path_dlg_xp(hwndOwner));
    }
    return ans;
}

vector<CString> get_files_in_path(CString path)
{
    vector<CString> ans;
    CFileFind ff;
    BOOL b = ff.FindFile(path + _T("\\*.*"));
    while (b) {
        b = ff.FindNextFile();
        CString str = ff.GetFilePath();
        if (ff.IsDots() || ff.IsHidden()) {
            continue;
        } else if (ff.IsDirectory()) {
            append(get_files_in_path(str), ans);
        } else {
            append(str, ans);
        }
    }
    return ans;
}

CString YYMMDD(CTime t)
{
    CString str;
    str.Format(_T("%4d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
    return str;
}

CString hhmmss(CTime t)
{
    CString str;
    str.Format(_T("%02d%02d%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());
    return str;
}

CString change_word_case(CString str, int wcase)
{
    if (str.IsEmpty())
        return _T("");
    CString ans = str;
    //不变
    if (wcase == 0) {
    }
    //全部大写
    else if (wcase == 1) {
        ans = str.MakeUpper();
    }
    //全部小写
    else if (wcase == 2) {
        ans = str.MakeLower();
    }
    //首字母大写
    else if (wcase == 3) {
        ans = str.MakeLower();
        CString t1 = ans.Left(1);
        CString t2 = ans.Right(ans.GetLength() - 1);
        ans = t1.MakeUpper() + t2;
    }
    //每个词的首字母大写
    else if (wcase == 4) {
        bool balpha = false;
        for (int i = 0; i < str.GetLength(); ++i) {
            TCHAR c = str[i];
            if ((c >= _T('a') && c <= _T('z')) || (c >= _T('A') && c <= _T('Z'))) {
                c = balpha ? tolower(c) : toupper(c);
                str.SetAt(i, c);
                balpha = true;
            } else {
                balpha = false;
            }
        }
        ans = str;
    }
    return ans;
}