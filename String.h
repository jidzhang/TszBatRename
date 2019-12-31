#pragma once

namespace String
{
	CString		ToCString(const std::string & str);
	std::string ToString(LPCTSTR str);
	std::string ToString(int v);
	std::string ToString(long v);
	std::string ToString(double v, int dig = -1);

	std::string ToUpper(const std::string & str);
	std::string ToLower(const std::string & str);

	int			ToInt(const std::string & str);
	double		ToDouble(const std::string & str);

	std::vector<std::string> Split(const std::string & data, const std::string & delimiters = " \t\r\n");

	CString LoadString(UINT nID);
}

CString JoinPath(LPCTSTR str1, LPCTSTR str2);
