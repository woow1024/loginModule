 #pragma once

#include <tchar.h>

// 键盘精灵 - 快捷键 
const UINT MSG_SHORTKEY				= ::RegisterWindowMessage(_T("MSG_SHORTKEY"));	
// 键盘精灵 - 切换代码
const UINT MSG_CHANGESYMBOL			= ::RegisterWindowMessage(_T("MSG_CHANGESYMBOL"));	
// 通知父窗体当前选中市场变化
const UINT MSG_CHANGEMARKET			= ::RegisterWindowMessage(_T("MSG_CHANGEMARKET"));
// 设置当前keyview
const UINT MSG_SETKEYVIEW			= ::RegisterWindowMessage(_T("MSG_SETKEYVIEW"));

// 视图切换 - 工具栏，WPARAM(LPCTSTR) - view类名， LPARAM(LPCTSTR) - 参数		
const UINT MSG_SWITCHVIEW			= ::RegisterWindowMessage(_T("MSG_SWITCHVIEW"));

// 重新加载雷达
const UINT MSG_RADAR_CHANGE		= ::RegisterWindowMessage(_T("MSG_RADAR_CHANGE"));
// 删除雷达
const UINT MSG_RADAR_DEL		= ::RegisterWindowMessage(_T("MSG_RADAR_DEL"));
// 新建雷达
const UINT MSG_RADAR_ADD		= ::RegisterWindowMessage(_T("MSG_RADAR_ADD"));

// 主程序启动升级进程
const UINT MSG_UPGRADE_PROC		= ::RegisterWindowMessage(_T("MSG_UPGRADE_PROC"));

// GWMessage
const UINT MSG_GWSTATION_MSG	= ::RegisterWindowMessage(_T("MSG_GWSTATION_MSG"));

enum
{
	GWSTATION_MSG_UICHANGED			= 101,
	GWSTATION_MSG_PARENTVIEW_SHOW	= 102,			// 父窗体准备显示/隐藏	
	GWSTATION_MSG_ROOM_CHANGED		= 103,			//直播室改变
};

// 向主框架申请创建新view， WPARAM - PGWCREATEVIEWINFO， LPARAM - 0 Send 1 Post
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

// 想主框架请求销毁视图， WPARAM - 视图指针，必须使用SendMessage
const UINT MSG_DESTROYVIEW			= ::RegisterWindowMessage(_T("MSG_DESTROYVIEW"));
// 想主框架请求销毁视图， WPARAM - 对话框，必须使用SendMessage
const UINT MSG_DESTROYDLG				= ::RegisterWindowMessage(_T("MSG_DESTROYDLG"));
// 收到新数据包， WPARAM - 数据包
const UINT MSG_NEWPACK_RECV			= ::RegisterWindowMessage("MSG_NEWPACK_RECV");
// 向主框架申请键盘精灵窗体,必须使用SendMessage
const UINT MSG_QUERYKEYWIZZARD		= ::RegisterWindowMessage("MSG_QUERYKEYWIZZARD");
// 要求获取当前的SymbolObj	WPARAM =  分组
const UINT MSG_QUERYMAINSYMBOLOBJ	= ::RegisterWindowMessage("MSG_QUERYMAINSYMBOLOBJ");

// 向父窗体要求全屏(相对父窗体)显示
const UINT MSG_FULLSCREENSHOW		= ::RegisterWindowMessage("MSG_FULLSCREENSHOW");

// 子窗体大小状态发生自发变化
const UINT MSG_SUBWINDOWSTATECHANGE = ::RegisterWindowMessage(_T("MSG_SUBWINDOWSTATECHANGE"));

//向主框架申请View入Backspace栈
const UINT MSG_SWITCHVIEW_INTOSTACK	= ::RegisterWindowMessage(_T("MSG_SWITCHVIEW_INTOSTACK"));

// 直接命令（可配置菜单，工具栏），WPARAM - DirectCommand 
const UINT MSG_DIRECT_COMMAND		= ::RegisterWindowMessage("MSG_DIRECT_COMMAND");

// 帐号重复登录
const UINT MSG_ACCOUNT_REPEAT_LOGIN = ::RegisterWindowMessage(_T("MSG_ACCOUNT_REPEAT_LOGIN"));

// Web登录结束，通知主框架销毁登录视图, WPARAM视图指针，LPARAM WebKernelType
enum WebKernelType { IEKernel = 0, WebkitKernel };
const UINT MSG_WEBLOGIN_FINISH		= ::RegisterWindowMessage(_T("MSG_WEBLOGIN_FINISH"));

// 当前页面加载完毕
const UINT MSG_URLLOAD_FINISH		= ::RegisterWindowMessage(_T("MSG_URLLOAD_FINISH"));

// 当前页面加载完毕
const UINT MSG_WEBVIEW_TITLE_CHANGED		= ::RegisterWindowMessage(_T("MSG_WEBVIEW_TITLE_CHANGED"));


// 当前页面关闭
const UINT MSG_WEBVIEW_DOCLOSE		= ::RegisterWindowMessage(_T("MSG_WEBVIEW_DOCLOSE"));

// 网页相关的宏定义
const UINT MSG_WEB_JUMP				= ::RegisterWindowMessage(_T("MSG_WEB_JUMP"));	
const UINT MSG_IEWEB_LOAD_NOTIFY			= ::RegisterWindowMessage(_T("MSG_IEWEB_LOAD_NOTIFY"));	
#define  WEB_EXPLORER_QUIT_URL							"close.html"													// 网页退出URL
#define  WEB_EXPLORER_QUIT_URL2						"close.html/"			
#define  WEB_EXPLORER_SUCCESS_URL						"success.html"													// 网页操作成功URL
#define  WEB_EXPLORER_SUCCESS_URL2						"success.html/"			
#define WEB_EXPLORER_JUMP2COMMONPROBLEM			"commonproblem.html"				//常见问题跳转路径
#define WEB_EXPLORER_JUMP2COMMONPROBLEM2			"commonproblem.html/"				//常见问题跳转路径
#define WEB_EXPLORER_JUMP2FEEKBACK						"feekback.html"						//问题反馈跳转路径
#define WEB_EXPLORER_JUMP2FEEKBACK2					"feekback.html/"						//问题反馈跳转路径



#define  WM_SYS_MESAGE_BASE						(WM_USER + 2012)

#define  WM_POPUP_SYSMENU_NOTIFY				(WM_SYS_MESAGE_BASE + 2)	//系统菜单弹出
#define  WM_MSG_BASEDLG_CLOSE					(WM_SYS_MESAGE_BASE + 3)

#define  WM_MSG_COMMON_POPUP_URL				 (WM_SYS_MESAGE_BASE + 5)	//普通的外跳网页， WPARAM 标题， LPARAM 网页URL
#define  WM_MSG_MYTRADER_RELOGON				 (WM_SYS_MESAGE_BASE + 6)	//会员重新登录
#define  WM_MSG_MYTRADER_REGISTER				 (WM_SYS_MESAGE_BASE + 7)	//会员注册
#define  WM_MSG_CALL_SERVER						 (WM_SYS_MESAGE_BASE + 8)	//电话服务

#define  WM_MSG_SELFSTOCK_CHANGE				(WM_SYS_MESAGE_BASE + 10)	//自选股改变
#define  WM_MSG_NET_DISCONNECT					(WM_SYS_MESAGE_BASE + 100)	//NET 重连失败
#define  WM_MSG_NET_CLEARLOCALDATA				(WM_SYS_MESAGE_BASE + 101)	//清除本地数据

#define  WM_MSG_LSP_REF								(WM_SYS_MESAGE_BASE + 102)		// LSP营销消息广播(查价框) by carrot


// 自销毁子窗口销毁时发消息到父窗口
const UINT MSG_CHILDWND_SELFDESTORY		= ::RegisterWindowMessage(_T("MSG_CHILDWND_SELFDESTORY"));