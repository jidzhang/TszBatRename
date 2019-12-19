#include "stdafx.h"
#include "Settings.h"

using namespace std;
using namespace String;

static CSettings* g_setting = NULL;

CSettings::CSettings(std::string appName, CWinApp* pApp)
{
	init(ToCString(appName), pApp);
}

CSettings::CSettings(CString appName, CWinApp* pApp /*= NULL*/)
{
	init(appName, pApp);
}

CSettings::~CSettings()
{
}

void CSettings::Reset()
{
	if (!m_pApp)
		return;
	m_pApp->WriteProfileString(m_appName, NULL, NULL);
}

void CSettings::init(CString appName, CWinApp* pApp)
{
	m_pApp = pApp ? pApp : AfxGetApp();
	m_appName = appName;
	if (m_appName.IsEmpty()) {
		m_appName = _T("TszBatRename");
	}
}

void CSettings::Save(std::string key, int v)
{
	Save(ToCString(key), v);
}

void CSettings::Save(std::string key, std::string v)
{
	Save(ToCString(key), ToCString(v));
}

void CSettings::Save(CString key, int v)
{
	if (!m_pApp)
		return;
	m_pApp->WriteProfileInt(m_appName, key, v);
}

void CSettings::Save(CString key, CString v)
{
	if (!m_pApp)
		return;
	m_pApp->WriteProfileString(m_appName, key, v);
}

void CSettings::Save(CString key, double v)
{
	if (!m_pApp)
		return;
	CString str;
	str.Format(_T("%f"), v);
	m_pApp->WriteProfileString(m_appName, key, str);
}

void CSettings::Load(std::string key, int& v)
{
	if (!m_pApp)
		return;
	v = m_pApp->GetProfileInt(m_appName, ToCString(key), v);
}

void CSettings::Load(std::string key, std::string& v)
{
	if (!m_pApp)
		return;
	CString s = m_pApp->GetProfileString(m_appName, ToCString(key));
	v = ToString(s);
}

void CSettings::Load(CString key, int& v)
{
	if (!m_pApp)
		return;
	v = m_pApp->GetProfileInt(m_appName, key, v);
}

void CSettings::Load(CString key, CString& v)
{
	if (!m_pApp)
		return;
	v = m_pApp->GetProfileString(m_appName, key, v);
}

void CSettings::Load(CString key, double& v)
{
	if (!m_pApp)
		return;
	CString str;
	str = m_pApp->GetProfileString(m_appName, key, str);
	if (!str.IsEmpty())
		v = _tstof(str);
}

void InitSetting(CString appName /*= _T("")*/, CWinApp* pApp /*= NULL*/)
{
	if (g_setting) {
		delete g_setting;
		g_setting = 0;
	}
	g_setting = new CSettings(appName, pApp);
}

void SaveSetting(CString key, int v)
{
	if (!g_setting)
		InitSetting();
	g_setting->Save(key, v);
}

void SaveSetting(CString key, CString v)
{
	if (!g_setting)
		InitSetting();
	g_setting->Save(key, v);
}

void SaveSetting(CString key, double v)
{
	if (!g_setting)
		InitSetting();
	g_setting->Save(key, v);
}

void LoadSetting(CString key, int& v)
{
	if (!g_setting)
		InitSetting();
	g_setting->Load(key, v);
}

void LoadSetting(CString key, CString& v)
{
	if (!g_setting)
		InitSetting();
	g_setting->Load(key, v);
}

void LoadSetting(CString key, double& v)
{
	if (!g_setting)
		InitSetting();
	g_setting->Load(key, v);
}

void ResetSetting()
{
	if (!g_setting)
		InitSetting();
	g_setting->Reset();
}

void DestorySetting()
{
	if (g_setting) {
		delete g_setting;
		g_setting = 0;
	}
}