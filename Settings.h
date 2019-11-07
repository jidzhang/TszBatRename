#pragma once
class CSettings
{
public:
	CSettings(std::string appName, CWinApp * pApp = NULL);
	~CSettings();
	void Save(std::string key, int v);
	void Save(std::string key, std::string v);
	void Load(std::string key, int & v);
	void Load(std::string key, std::string & v);
private:
	CWinApp * m_pApp;
	std::string m_appName;
};
