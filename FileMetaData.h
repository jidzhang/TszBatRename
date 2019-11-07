#pragma once
class CFileMetaData
{
public:
    CFileMetaData(CString filePath = _T(""));
    ~CFileMetaData();
    bool Status();

public:
    bool m_status;
    int  m_sort_idx;
    bool m_checked;
    CString m_file_path;
    CString m_file_name;
    CString m_file_ext;
    CString m_file_size;
    CString m_file_time;
    CString m_file_dir;
    CString m_file_new;
};
