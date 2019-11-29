#include "stdafx.h"
#include "FileMetaData.h"

CFileMetaData::CFileMetaData(CString filePath)
{
	m_status = false;
	//
	m_sort_idx = 0;
	m_file_path = _T("");
	m_file_name = _T("");
	m_file_ext = _T("");
	m_file_size = _T("");
	m_file_time = _T("");
	m_file_dir = _T("");
	m_checked = true;
	//
	if (filePath.IsEmpty())
		return;
	if (filePath[0] == _T('"')) {
		filePath = filePath.Right(filePath.GetLength() - 1);
	}
	if (filePath.IsEmpty())
		return;
	if (filePath[filePath.GetLength() - 1] == _T('"')) {
		filePath = filePath.Left(filePath.GetLength() - 1);
	}
	if (filePath.IsEmpty())
		return;
	m_file_path = filePath;
	int p = m_file_path.ReverseFind('\\');
	if (p > 0) {
		m_file_dir = m_file_path.Left(p + 1);
		m_file_name = m_file_path.Right(m_file_path.GetLength() - p - 1);
		p = m_file_name.ReverseFind('.');
		if (p > 0) {
			m_file_ext = m_file_name.Right(m_file_name.GetLength() - p - 1);
			m_file_name = m_file_name.Left(p);
		} else {
			return;
		}
	} else {
		return;
	}
	//
	WIN32_FIND_DATA findFileData;
	HANDLE h = FindFirstFile(m_file_path, &findFileData);
	if (h > 0) {
		__int64 file_size = (__int64)findFileData.nFileSizeHigh << sizeof(DWORD) * 8;
		file_size += findFileData.nFileSizeLow;
		m_file_size.Format(_T("%u"), file_size);
		FILETIME fTime = findFileData.ftLastWriteTime;
		SYSTEMTIME st = { 0 };
		FileTimeToSystemTime(&fTime, &st);
		m_file_time.Format(_T("%4d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour + 8, st.wMinute, st.wSecond);
		m_status = true;
	}
}

CFileMetaData::~CFileMetaData()
{
}

bool CFileMetaData::Status()
{
	return m_status;
}