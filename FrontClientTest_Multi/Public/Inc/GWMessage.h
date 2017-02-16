 #pragma once

#include <tchar.h>

// ���̾��� - ��ݼ� 
const UINT MSG_SHORTKEY				= ::RegisterWindowMessage(_T("MSG_SHORTKEY"));	
// ���̾��� - �л�����
const UINT MSG_CHANGESYMBOL			= ::RegisterWindowMessage(_T("MSG_CHANGESYMBOL"));	
// ֪ͨ�����嵱ǰѡ���г��仯
const UINT MSG_CHANGEMARKET			= ::RegisterWindowMessage(_T("MSG_CHANGEMARKET"));
// ���õ�ǰkeyview
const UINT MSG_SETKEYVIEW			= ::RegisterWindowMessage(_T("MSG_SETKEYVIEW"));

// ��ͼ�л� - ��������WPARAM(LPCTSTR) - view������ LPARAM(LPCTSTR) - ����		
const UINT MSG_SWITCHVIEW			= ::RegisterWindowMessage(_T("MSG_SWITCHVIEW"));

// ���¼����״�
const UINT MSG_RADAR_CHANGE		= ::RegisterWindowMessage(_T("MSG_RADAR_CHANGE"));
// ɾ���״�
const UINT MSG_RADAR_DEL		= ::RegisterWindowMessage(_T("MSG_RADAR_DEL"));
// �½��״�
const UINT MSG_RADAR_ADD		= ::RegisterWindowMessage(_T("MSG_RADAR_ADD"));

// ������������������
const UINT MSG_UPGRADE_PROC		= ::RegisterWindowMessage(_T("MSG_UPGRADE_PROC"));

// GWMessage
const UINT MSG_GWSTATION_MSG	= ::RegisterWindowMessage(_T("MSG_GWSTATION_MSG"));

enum
{
	GWSTATION_MSG_UICHANGED			= 101,
	GWSTATION_MSG_PARENTVIEW_SHOW	= 102,			// ������׼����ʾ/����	
	GWSTATION_MSG_ROOM_CHANGED		= 103,			//ֱ���Ҹı�
};

// ����������봴����view�� WPARAM - PGWCREATEVIEWINFO�� LPARAM - 0 Send 1 Post
typedef struct tagGWCREATEVIEWINFO
{
	CString strClassName;
	CString strParam;
	CWnd*	pParent;
	CRect	rcWnd;
	UINT	uID;
	int		nGroup;

	tagGWCREATEVIEWINFO()
	{
		pParent	= NULL;
		uID		= (UINT)-1;
		nGroup	= 0;
		rcWnd.SetRectEmpty();
	}
} GWCREATEVIEWINFO, *PGWCREATEVIEWINFO, *LPGWCREATEVIEWINFO;

const UINT MSG_CREATEVIEW			= ::RegisterWindowMessage(_T("MSG_CREATEVIEW"));

// �����������������ͼ�� WPARAM - ��ͼָ�룬����ʹ��SendMessage
const UINT MSG_DESTROYVIEW			= ::RegisterWindowMessage(_T("MSG_DESTROYVIEW"));
// �����������������ͼ�� WPARAM - �Ի��򣬱���ʹ��SendMessage
const UINT MSG_DESTROYDLG				= ::RegisterWindowMessage(_T("MSG_DESTROYDLG"));
// �յ������ݰ��� WPARAM - ���ݰ�
const UINT MSG_NEWPACK_RECV			= ::RegisterWindowMessage("MSG_NEWPACK_RECV");
// �������������̾��鴰��,����ʹ��SendMessage
const UINT MSG_QUERYKEYWIZZARD		= ::RegisterWindowMessage("MSG_QUERYKEYWIZZARD");
// Ҫ���ȡ��ǰ��SymbolObj	WPARAM =  ����
const UINT MSG_QUERYMAINSYMBOLOBJ	= ::RegisterWindowMessage("MSG_QUERYMAINSYMBOLOBJ");

// �򸸴���Ҫ��ȫ��(��Ը�����)��ʾ
const UINT MSG_FULLSCREENSHOW		= ::RegisterWindowMessage("MSG_FULLSCREENSHOW");

// �Ӵ����С״̬�����Է��仯
const UINT MSG_SUBWINDOWSTATECHANGE = ::RegisterWindowMessage(_T("MSG_SUBWINDOWSTATECHANGE"));

//�����������View��Backspaceջ
const UINT MSG_SWITCHVIEW_INTOSTACK	= ::RegisterWindowMessage(_T("MSG_SWITCHVIEW_INTOSTACK"));

// ֱ����������ò˵�������������WPARAM - DirectCommand 
const UINT MSG_DIRECT_COMMAND		= ::RegisterWindowMessage("MSG_DIRECT_COMMAND");

// �ʺ��ظ���¼
const UINT MSG_ACCOUNT_REPEAT_LOGIN = ::RegisterWindowMessage(_T("MSG_ACCOUNT_REPEAT_LOGIN"));

// Web��¼������֪ͨ��������ٵ�¼��ͼ, WPARAM��ͼָ�룬LPARAM WebKernelType
enum WebKernelType { IEKernel = 0, WebkitKernel };
const UINT MSG_WEBLOGIN_FINISH		= ::RegisterWindowMessage(_T("MSG_WEBLOGIN_FINISH"));

// ��ǰҳ��������
const UINT MSG_URLLOAD_FINISH		= ::RegisterWindowMessage(_T("MSG_URLLOAD_FINISH"));

// ��ǰҳ��������
const UINT MSG_WEBVIEW_TITLE_CHANGED		= ::RegisterWindowMessage(_T("MSG_WEBVIEW_TITLE_CHANGED"));


// ��ǰҳ��ر�
const UINT MSG_WEBVIEW_DOCLOSE		= ::RegisterWindowMessage(_T("MSG_WEBVIEW_DOCLOSE"));

// ��ҳ��صĺ궨��
const UINT MSG_WEB_JUMP				= ::RegisterWindowMessage(_T("MSG_WEB_JUMP"));	
const UINT MSG_IEWEB_LOAD_NOTIFY			= ::RegisterWindowMessage(_T("MSG_IEWEB_LOAD_NOTIFY"));	
#define  WEB_EXPLORER_QUIT_URL							"close.html"													// ��ҳ�˳�URL
#define  WEB_EXPLORER_QUIT_URL2						"close.html/"			
#define  WEB_EXPLORER_SUCCESS_URL						"success.html"													// ��ҳ�����ɹ�URL
#define  WEB_EXPLORER_SUCCESS_URL2						"success.html/"			
#define WEB_EXPLORER_JUMP2COMMONPROBLEM			"commonproblem.html"				//����������ת·��
#define WEB_EXPLORER_JUMP2COMMONPROBLEM2			"commonproblem.html/"				//����������ת·��
#define WEB_EXPLORER_JUMP2FEEKBACK						"feekback.html"						//���ⷴ����ת·��
#define WEB_EXPLORER_JUMP2FEEKBACK2					"feekback.html/"						//���ⷴ����ת·��



#define  WM_SYS_MESAGE_BASE						(WM_USER + 2012)

#define  WM_POPUP_SYSMENU_NOTIFY				(WM_SYS_MESAGE_BASE + 2)	//ϵͳ�˵�����
#define  WM_MSG_BASEDLG_CLOSE					(WM_SYS_MESAGE_BASE + 3)

#define  WM_MSG_COMMON_POPUP_URL				 (WM_SYS_MESAGE_BASE + 5)	//��ͨ��������ҳ�� WPARAM ���⣬ LPARAM ��ҳURL
#define  WM_MSG_MYTRADER_RELOGON				 (WM_SYS_MESAGE_BASE + 6)	//��Ա���µ�¼
#define  WM_MSG_MYTRADER_REGISTER				 (WM_SYS_MESAGE_BASE + 7)	//��Աע��
#define  WM_MSG_CALL_SERVER						 (WM_SYS_MESAGE_BASE + 8)	//�绰����

#define  WM_MSG_SELFSTOCK_CHANGE				(WM_SYS_MESAGE_BASE + 10)	//��ѡ�ɸı�
#define  WM_MSG_NET_DISCONNECT					(WM_SYS_MESAGE_BASE + 100)	//NET ����ʧ��
#define  WM_MSG_NET_CLEARLOCALDATA				(WM_SYS_MESAGE_BASE + 101)	//�����������

#define  WM_MSG_LSP_REF								(WM_SYS_MESAGE_BASE + 102)		// LSPӪ����Ϣ�㲥(��ۿ�) by carrot


// �������Ӵ�������ʱ����Ϣ��������
const UINT MSG_CHILDWND_SELFDESTORY		= ::RegisterWindowMessage(_T("MSG_CHILDWND_SELFDESTORY"));