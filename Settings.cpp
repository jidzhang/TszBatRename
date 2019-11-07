#include "stdafx.h"
#include "Settings.h"

using namespace std;
using namespace String;

CSettings::CSettings(std::string appName, CWinApp * pApp)
{
    m_pApp = pApp ? pApp : AfxGetApp();
    m_appName = appName;
}

CSettings::~CSettings()
{
}

void CSettings::Save(std::string key, int v)
{
    if (!m_pApp)
        return;
    m_pApp->WriteProfileInt(ToCString(m_appName), ToCString(key), v);
}

void CSettings::Save(std::string key, std::string v)
{
    if (!m_pApp)
        return;
    m_pApp->WriteProfileString(ToCString(m_appName), ToCString(key), ToCString(v));
}

void CSettings::Load(std::string key, int & v)
{
    if (!m_pApp)
        return;
    m_pApp->GetProfileInt(ToCString(m_appName), ToCString(key), v);
}

void CSettings::Load(std::string key, std::string & v)
{
    if (!m_pApp)
        return;
    LPCTSTR lpszV = NULL;
    m_pApp->GetProfileString(ToCString(m_appName), ToCString(key), lpszV);
    v = ToString(lpszV);
}