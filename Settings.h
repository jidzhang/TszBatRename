#pragma once
class CSettings
{
public:
	CSettings(std::string appName, CWinApp * pApp = NULL);
	CSettings(CString appName, CWinApp * pApp = NULL);
	~CSettings();
	void Reset();
	void Save(std::string key, int v);
	void Save(std::string key, std::string v);
	void Save(CString key, int v);
	void Save(CString key, double v);
	void Save(CString key, CString v);
	void Load(std::string key, int & v);
	void Load(std::string key, std::string & v);
	void Load(CString key, int & v);
	void Load(CString key, double & v);
	void Load(CString key, CString & v);
private:
	void init(CString appName, CWinApp * pApp = NULL);
private:
	CWinApp * m_pApp;
	CString m_appName;
};

void InitSetting(CString appName = _T(""), CWinApp * pApp = NULL);
void SaveSetting(CString key, int v);
void SaveSetting(CString key, double v);
void SaveSetting(CString key, CString v);
void LoadSetting(CString key, int & v);
void LoadSetting(CString key, double & v);
void LoadSetting(CString key, CString & v);
void ResetSetting();
void DestorySetting();
