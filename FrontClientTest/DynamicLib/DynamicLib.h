// DynamicLib.h : DynamicLib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDynamicLibApp
// �йش���ʵ�ֵ���Ϣ������� DynamicLib.cpp
//

class CDynamicLibApp : public CWinApp
{
public:
	CDynamicLibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
