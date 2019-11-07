#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

class CTszRenameApp : public CWinApp
{
public:
    CTszRenameApp();

    // 重写
public:
    virtual BOOL InitInstance();

    // 实现

    DECLARE_MESSAGE_MAP()
};

extern CTszRenameApp theApp;