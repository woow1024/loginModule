// dllmain.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#include "funclib.h"
#include <GdiPlus.h>
#include <BuildInfo.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE FuncLibDLL = { NULL, NULL };

#define SUPPORT_GDIPLUS

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

#ifdef SUPPORT_GDIPLUS
	using namespace Gdiplus;
	ULONG_PTR gdiplusToken = NULL;
#endif

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("FuncLib.DLL ���ڳ�ʼ��!\n");
		
#ifdef SUPPORT_GDIPLUS
		GdiplusStartupInput gdiplusStartupInput;
		Status status = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		if (status != Ok)
		{
			ASSERT(FALSE);
			WriteDebugMessage("��ʼ��GDIPLUS��ʧ�ܣ�����");
		}
#endif

		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(FuncLibDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL (�� ActiveX �ؼ�)��ʽ���ӵ���
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(FuncLibDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("FuncLib.DLL ������ֹ!\n");

#ifdef SUPPORT_GDIPLUS
		GdiplusShutdown(gdiplusToken);
#endif

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(FuncLibDLL);
	}
	return 1;   // ȷ��
}
