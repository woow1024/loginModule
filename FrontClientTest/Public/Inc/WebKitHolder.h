#pragma once

#ifdef GW_WEBKIT_DLL
#define  GW_WEBKIT_API __declspec( dllexport )		
#else
#define  GW_WEBKIT_API __declspec( dllimport )		
#endif

#include "cef_ptr.h"

// CWebKitWnd
#define  WEB_HELPER_OK						0	
#define  WEB_HELPER_FALSE					-1
#define  WEB_HELPER_ERROR_EMPTY				1
#define  WEB_HELPER_ERROR_NOLICENSE			2

#define  EMPTY_URL		L"about:blank"

BOOL GW_WEBKIT_API InitCefClient();
void GW_WEBKIT_API ShutDownCefClient();

class ClientHandler;
#define  USE_SELF_CREATE	0
class GW_WEBKIT_API CWebKitHolder
{
public:
	CWebKitHolder(void);
	~CWebKitHolder(void);
protected:
	CefRefPtr<ClientHandler> m_pClientHandler;
	WCHAR  m_szURL[512];
	HWND m_hMainWnd;
	CRect m_rcRect;
public:
	void SetMainWnd(HWND hWnd, CRect& rcRect);

	BOOL CreateWebCtrl();

	LONG LoadURL(LPCWSTR pszURL);

	void GoBack(void);
	void GoForward(void);
	void DoRefresh(void);

	LPCWSTR GetURL(void);
	void EnablePopUpWindow(LONG bEnablePopup);
	LONG StopLoad(void);
	LONG GetBrowserID(void);

	void AdustPosition(CRect & rcRect);
protected:
	BOOL DoNavigate();
};
