
// Display.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CDisplayApp:
// �� Ŭ������ ������ ���ؼ��� Display.cpp�� �����Ͻʽÿ�.
//

class CDisplayApp : public CWinApp
{
public:
	CDisplayApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDisplayApp theApp;