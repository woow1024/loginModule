#pragma once

class GWBasePack;

typedef enum tagConnectEventType
{
	CONNECT_REBUID	= 0,			// 重建连接成功
	CONNECT_DISCONNECTED,			// 连接中断
	IE_LOGON_FINISHED,				// IE登录完成
	WEBKIT_LOGON_FINISHED,			// WEBKIT登录完成
} GWConnectEventType;

#define BEING_GWINTERFACE(name)					interface name {
#define END_GWINTERFACE()						};
#define PUREMETHOD_(type, name, arglist)		virtual type name##arglist PURE;
#define PUREMETHOD(name, arglst)				PUREMETHOD_(void, name, arglst)

BEING_GWINTERFACE(IMessageRevcer)
	PUREMETHOD_(CString, GetRecverName, ())									// 获取接收者名称
	PUREMETHOD(OnDataPackRecv, (GWBasePack *pPack))							// 数据接收通知
	PUREMETHOD(OnConnectEvent, (GWConnectEventType nEvent))					// 连接事件通知
END_GWINTERFACE()

MIDL_INTERFACE("6C625C4D-FEAB-4a99-B081-FFD600297FB4")
IGWPlugin : public IUnknown{																							// 插件
public:
	STDMETHOD_(void, ChangeSymbolObj)(SymbolObj* pSymbolObj, int nGroups, CWnd* pSender) PURE;							// 当前商品改变
	STDMETHOD_(void, Activate)() PURE;																					// 激活插件
	STDMETHOD_(void, DeActivate)() PURE;																				// 进入休眠模式
	STDMETHOD_(BOOL, PreDestroy)() PURE;																				// 销毁前通知
	STDMETHOD_(CString, GetKey)() PURE;																					// 获取插件标识
	STDMETHOD_(void, ChangeRadarState)(SymbolObj* pSymbolObj,BOOL bActive, CWnd* pSender) PURE;							// 当前商品改变
};

MIDL_INTERFACE("454CB054-BB93-43e1-8E01-A0F4F6AF378A")
IGWViewPlugin : public IGWPlugin{																						// 视图插件
public:
	STDMETHOD_(void, ViewInitialize)(SymbolObj* pSymbolObj, const CString& strParam, int nGroups) PURE;					// 视图初始化	
	STDMETHOD_(BOOL, IsNeedPushInToViewStack)() PURE;																	// 视图是否需要出现在ViewStack中
	STDMETHOD_(CWnd*, GetActiveWnd)() PURE;																				// 获取当前的active wnd
	STDMETHOD_(BOOL, UIItemUpdate)(UINT uID, GWMenuItemEx* pUIItem) PURE;												// 界面元素更新状态
	STDMETHOD_(BOOL, EnableMessageHook)() PURE;																			// 是否需要父窗体消息过滤
	STDMETHOD_(CString, GetParamString)() PURE;																					// 获取插件标识
	STDMETHOD_(SymbolObj*, GetSymbol)() PURE;																					// 获取插件标识
};

MIDL_INTERFACE("0CCAD81D-7211-4c9e-9F60-00D14C2ED6CF")
IGWDlgPlugin : public IGWPlugin{																						// 对话框插件
public:
	STDMETHOD_(void, DlgInitialize)(SymbolObj* pSymbolObj, const CString& strParam) PURE;								// 对话框初始化																				// 视图是否需要出现在ViewStack中
};

typedef BOOL (*RegisterFramePlugin)(CWnd* pWnd);
typedef BOOL (__stdcall *PreTranslateMessageEntry)(MSG* pMsg);

BEING_GWINTERFACE(IPluginFactory)
	PUREMETHOD_(BOOL, RegisterFramePlugin, (CRuntimeClass *pClass))											// 注册框架插件
	PUREMETHOD_(BOOL, RegisterViewClass, (CRuntimeClass *pClass, const CString& strCName))					// 注册可动态创建类
	PUREMETHOD_(BOOL, RegisterMsgEntry, (PreTranslateMessageEntry pfnMsgEntry))								// 注册可动态创建类
	PUREMETHOD_(BOOL, RegisterDlgClass, (CRuntimeClass *pClass, const CString& strCName))					// 注册可动态创建对话框类
END_GWINTERFACE()

typedef BOOL (*GWInitPlugin)(IPluginFactory* pPluginFactory);										// 插件初始化函数

const char PLUGIN_INITFUNC_NAME[] = "PluginInitialize";

#define DECLARE_PLUGIN_ENTRY()	\
	IPluginFactory* g_pPluginFactory = NULL;		\
	IPluginFactory * AfxGetPluginFoctory()		\
{				\
	return g_pPluginFactory;				\
}	\
	extern "C" __declspec(dllexport) BOOL PluginInitialize(IPluginFactory* pPluginFactory)  \
{	\
	g_pPluginFactory = pPluginFactory;	

#define REGISTER_FRAMEPLUGIN(classname)	\
	g_pPluginFactory->RegisterFramePlugin(RUNTIME_CLASS(classname));

#define REGISTER_VIEWCLASS(classname, cname)		\
	g_pPluginFactory->RegisterViewClass(RUNTIME_CLASS(classname), cname);

#define REGISTER_DLGCLASS(classname, cname)		\
	g_pPluginFactory->RegisterDlgClass(RUNTIME_CLASS(classname), cname);

#define REGISTER_MSGENTRY(entry)	\
	g_pPluginFactory->RegisterMsgEntry(entry);

#define END_PLUGIN_ENTRY()	 \
	return TRUE;	\
}

typedef enum FramePlugInType
{
	FRAME_MENUBAR			= 0,				// 菜单
	FRAME_TOOLBAR			= 1,				// 工具栏
	FRAME_STATUSBAR			= 2,				// 状态栏
	FRAME_USERPLUGIN		= 3					// 用户自定义插件
} GWFramePlugInType;

MIDL_INTERFACE("7186AC7C-E029-4f3e-811D-5DF892FE7615")
IFramePlugin : public IUnknown																							// 框架插件
{																							
public:
	STDMETHOD_(BOOL, CreatePlugin)(CWnd* pParent, UINT uID) PURE;														// 创建
	STDMETHOD_(void, DestoryPlugin)() PURE;																				// 销毁
	STDMETHOD_(void, Reposition)(CRect& rcClient) PURE;																	// 重定位
	STDMETHOD_(GWFramePlugInType, GetPlugInType)() PURE;																// 插件类型
	STDMETHOD_(BOOL, IsSupportDock)() PURE;																				// 是否支持浮动
	STDMETHOD_(BOOL, ProceedCommand)(const CString& strCommand) PURE;													// 处理快捷键命令
	STDMETHOD_(void, ReLoad)() PURE;																					// 重新加载配置文件
};

#define GW_PREPAREVAR() \
	PARENTCLASS* pThis = \
	((PARENTCLASS*)((BYTE*)this - OffestThis())); \
	AFX_MANAGE_STATE(pThis->m_pModuleState) \
	pThis; // avoid warning from compiler \

#define BEGIN_GW_INTERFACE_PART(ownerClass, localClass)	\
class X##localClass : public localClass \
{ \
public: \
	STDMETHOD_(ULONG, AddRef)()		\
{			\
	GW_PREPAREVAR();		\
	return pThis->ExternalAddRef();			\
};	\
	STDMETHOD_(ULONG, Release)()	\
{	\
	GW_PREPAREVAR();		\
	return pThis->ExternalRelease();			\
};	\
	STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppvObj)		\
{	\
	GW_PREPAREVAR();		\
	return pThis->ExternalQueryInterface(&iid, ppvObj);	\
};	\
	typedef ownerClass	PARENTCLASS;	\
	int OffestThis() { return offsetof(ownerClass, m_x##localClass); }	

#define GW_IMPLEMENT_OBJ_INTERFACE_METHOD(name, params, args)	\
	STDMETHOD_(void, name)params	\
{	\
	GW_PREPAREVAR();	\
	pThis->name##args;			\
};

#define GW_IMPLEMENT_OBJ_INTERFACE_METHOD_EX(type, name, params, args)	\
	STDMETHOD_(type, name)params	\
{	\
	GW_PREPAREVAR();	\
	type ret = pThis->name##args;			\
	return ret;								\
};

#define END_GW_INTERFACE_PART(baseClass)					END_INTERFACE_PART(baseClass)		

#define GW_INTERFACE_PART(theClass, baseInterface)			INTERFACE_PART(theClass,  __uuidof(baseInterface), baseInterface)	

#define IMPLEMENT_OBJ_IUNKNOWN_INTERFACE(theClass, baseInterface)		\
	STDMETHODIMP theClass::X##baseInterface::QueryInterface(REFIID iid, LPVOID* ppvObj)	\
{	\
	METHOD_PROLOGUE(theClass, baseInterface);		\
	return pThis->ExternalQueryInterface(&iid, ppvObj);	\
}	\
	STDMETHODIMP_(ULONG) theClass::X##baseInterface::AddRef()	\
{	\
	METHOD_PROLOGUE(theClass, baseInterface);	\
	return pThis->ExternalAddRef();			\
}	\
	STDMETHODIMP_(ULONG) theClass::X##baseInterface::Release()	\
{	\
	METHOD_PROLOGUE(theClass, baseInterface);	\
	return pThis->ExternalRelease();	\
}

#define MAX_PFN_PARAMCOUNT		2

class FuntionNameCallAble;

typedef void (AFX_MSG_CALL FuntionNameCallAble::*PublicFunc)();

enum GWPublicFuncSign
{
	gw_pfn_v,
	gw_pfn_s,
	gw_pfn_i,
	gw_pfn_ss,
	gw_pfn_is,
	gw_pfn_si,


	gw_pfn_end
};

struct PublicFuncInfo
{
	CString				strFuncName;
	GWPublicFuncSign	eSign;
	PublicFunc			pFunc;
};

class FuntionNameCallAble
{
public:
	virtual const PublicFuncInfo* GetGWFuncMap() const { return NULL; }
};


#define BEGIN_GW_FUNCTION_MAP(thisClass)		\
	const PublicFuncInfo* thisClass::GetGWFuncMap()	const	\
{	\
	typedef thisClass	mainClass;	\
	typedef void (AFX_MSG_CALL CMainFrame::*mainPFN)();	\
	static PublicFuncInfo PublicFuncTable[] = {

#define ADD_FUNCTION(name)		ADD_FUNCTION_(name, gw_pfn_v)

#define ADD_FUNCTION_(name, sign)	\
{ #name, sign, (PublicFunc)(mainPFN)&CMainFrame::name },

#define END_GW_FUNCTION_MAP()	\
{ _T(""), gw_pfn_end, NULL} };	\
	return &PublicFuncTable[0];	\
}

#define DECLAR_GW_FUNCTION_MAP()	\
	virtual const PublicFuncInfo* GetGWFuncMap() const; 

union GWMessageMapFunctions
{
	PublicFunc pfunc;
	void (AFX_MSG_CALL FuntionNameCallAble::*pfn_i)(int);
	void (AFX_MSG_CALL FuntionNameCallAble::*pfn_s)(const CString&);
	void (AFX_MSG_CALL FuntionNameCallAble::*pfn_is)(int, const CString&);
	void (AFX_MSG_CALL FuntionNameCallAble::*pfn_si)(const CString&, int);
	void (AFX_MSG_CALL FuntionNameCallAble::*pfn_ss)(const CString&, const CString&);
};