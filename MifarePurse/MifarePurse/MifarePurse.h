
// MifarePurse.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMifarePurseApp:
// �йش����ʵ�֣������ MifarePurse.cpp
//

class CMifarePurseApp : public CWinApp
{
public:
	CMifarePurseApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMifarePurseApp theApp;