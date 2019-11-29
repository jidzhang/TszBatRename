#include "stdafx.h"
#include "String.h"

namespace String
{
	BOOL WCharToMByte(const wchar_t* lpcwszStr, char** lpszStr)
	{
		DWORD dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, NULL, 0, NULL, FALSE);
		if (dwMinSize > 0) {
			char* sText = new char[dwMinSize];
			if (!sText) {
				delete[]sText; sText = NULL;
				dwMinSize = 0;
			}
			if (dwMinSize > 0) {
				WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, sText, dwMinSize, NULL, FALSE);
				if (lpszStr)
					*lpszStr = sText;
			}
		}
		return dwMinSize;
	}

	DWORD MByteToWChar(const char* lpcszStr, wchar_t** lpwszStr)
	{
		DWORD dwMinSize = MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, NULL, 0);
		if (dwMinSize > 0) {
			wchar_t* pwText = new wchar_t[dwMinSize];
			if (!pwText) {
				delete[]pwText; pwText = NULL;
				dwMinSize = 0;
			}
			if (dwMinSize) {
				MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, pwText, dwMinSize);
				if (lpwszStr)
					*lpwszStr = pwText;
			}
		}
		return dwMinSize;
	}

	CString ToCString(const std::string& str)
	{
		CString strRe;
#ifdef _UNICODE
		wchar_t* ch = NULL;
		MByteToWChar(str.c_str(), &ch);
		if (ch)
			strRe = ch;
		delete ch; ch = NULL;
#else
		strRe = str.c_str();
#endif
		return strRe;
	}

	std::string ToString(LPCTSTR str)
	{
		std::string strRe;
		CString strSrc(str);
#if _UNICODE
		char* ch = NULL;
		WCharToMByte(strSrc.GetBuffer(strSrc.GetLength() + 1), &ch);
		if (ch) {
			strRe = ch;
		}
		delete[] ch;
		ch = NULL;
		strSrc.ReleaseBuffer(strSrc.GetLength() + 1);
#else
		strRe = strSrc.GetBuffer(strSrc.GetLength() + 1);
		strSrc.ReleaseBuffer(strSrc.GetLength() + 1);
#endif
		return strRe;
	}

	std::string ToString(int v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	std::string ToString(long v)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	std::string ToString(double v, int dig/*=-1*/)
	{
		stringstream ss;
		ss << v;
		return ss.str();
	}

	std::string ToUpper(const std::string& v)
	{
		std::string result = "";
		std::transform(v.begin(), v.end(), back_inserter(result), ::toupper);
		return result;
	}

	std::string ToLower(const std::string& v)
	{
		std::string result = "";
		std::transform(v.begin(), v.end(), back_inserter(result), ::tolower);
		return result;
	}

	int ToInt(const std::string& str)
	{
		return atoi(str.c_str());
	}

	double ToDouble(const std::string& str)
	{
		return atof(str.c_str());
	}

	std::vector<std::string> Split(const std::string& data, const std::string& delimiters/*=" \t\r\n"*/)
	{
		std::vector<std::string> items;
		std::string::size_type start = 0;
		std::string::size_type pos = data.find_first_of(delimiters, start);
		while (pos != std::string::npos) {
			if (pos != start)
				items.push_back(std::string(data, start, pos - start).c_str());
			start = pos + 1;
			pos = data.find_first_of(delimiters, start);
		}
		if (start < data.length()) {
			items.push_back(std::string(data, start, data.length() - start).c_str());
		}
		return items;
	}

	CString LoadString(UINT nID)
	{
		CString str;
		str.LoadString(nID);
		return str;
	}
}