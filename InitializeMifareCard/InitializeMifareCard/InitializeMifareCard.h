
// InitializeMifareCard.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CInitializeMifareCardApp:
// �йش����ʵ�֣������ InitializeMifareCard.cpp
//

class CInitializeMifareCardApp : public CWinApp
{
public:
	CInitializeMifareCardApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CInitializeMifareCardApp theApp;