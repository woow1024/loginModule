#pragma once

class GWBasePack;

typedef enum tagConnectEventType
{
	CONNECT_REBUID	= 0,			// �ؽ����ӳɹ�
	CONNECT_DISCONNECTED,			// �����ж�
	IE_LOGON_FINISHED,				// IE��¼���
	WEBKIT_LOGON_FINISHED,			// WEBKIT��¼���
} GWConnectEventType;

#define BEING_GWINTERFACE(name)					interface name {
#define END_GWINTERFACE()						};
#define PUREMETHOD_(type, name, arglist)		virtual type name##arglist PURE;
#define PUREMETHOD(name, arglst)				PUREMETHOD_(void, name, arglst)

BEING_GWINTERFACE(IMessageRevcer)
	PUREMETHOD_(CString, GetRecverName, ())									// ��ȡ����������
	PUREMETHOD(OnDataPackRecv, (GWBasePack *pPack))							// ���ݽ���֪ͨ
	PUREMETHOD(OnConnectEvent, (GWConnectEventType nEvent))					// �����¼�֪ͨ
END_GWINTERFACE()

MIDL_INTERFACE("6C625C4D-FEAB-4a99-B081-FFD600297FB4")
IGWPlugin : public IUnknown{																							// ���
public:
	STDMETHOD_(void, ChangeSymbolObj)(SymbolObj* pSymbolObj, int nGroups, CWnd* pSender) PURE;							// ��ǰ��Ʒ�ı�
	STDMETHOD_(void, Activate)() PURE;																					// ������
	STDMETHOD_(void, DeActivate)() PURE;																				// ��������ģʽ
	STDMETHOD_(BOOL, PreDestroy)() PURE;																				// ����ǰ֪ͨ
	STDMETHOD_(CString, GetKey)() PURE;																					// ��ȡ�����ʶ
	STDMETHOD_(void, ChangeRadarState)(SymbolObj* pSymbolObj,BOOL bActive, CWnd* pSender) PURE;							// ��ǰ��Ʒ�ı�
};

MIDL_INTERFACE("454CB054-BB93-43e1-8E01-A0F4F6AF378A")
IGWViewPlugin : public IGWPlugin{																						// ��ͼ���
public:
	STDMETHOD_(void, ViewInitialize)(SymbolObj* pSymbolObj, const CString& strParam, int nGroups) PURE;					// ��ͼ��ʼ��	
	STDMETHOD_(BOOL, IsNeedPushInToViewStack)() PURE;																	// ��ͼ�Ƿ���Ҫ������ViewStack��
	STDMETHOD_(CWnd*, GetActiveWnd)() PURE;																				// ��ȡ��ǰ��active wnd
	STDMETHOD_(BOOL, UIItemUpdate)(UINT uID, GWMenuItemEx* pUIItem) PURE;												// ����Ԫ�ظ���״̬
	STDMETHOD_(BOOL, EnableMessageHook)() PURE;																			// �Ƿ���Ҫ��������Ϣ����
	STDMETHOD_(CString, GetParamString)() PURE;																					// ��ȡ�����ʶ
	STDMETHOD_(SymbolObj*, GetSymbol)() PURE;																					// ��ȡ�����ʶ
};

MIDL_INTERFACE("0CCAD81D-7211-4c9e-9F60-00D14C2ED6CF")
IGWDlgPlugin : public IGWPlugin{																						// �Ի�����
public:
	STDMETHOD_(void, DlgInitialize)(SymbolObj* pSymbolObj, const CString& strParam) PURE;								// �Ի����ʼ��																				// ��ͼ�Ƿ���Ҫ������ViewStack��
};

typedef BOOL (*RegisterFramePlugin)(CWnd* pWnd);
typedef BOOL (__stdcall *PreTranslateMessageEntry)(MSG* pMsg);

BEING_GWINTERFACE(IPluginFactory)
	PUREMETHOD_(BOOL, RegisterFramePlugin, (CRuntimeClass *pClass))											// ע���ܲ��
	PUREMETHOD_(BOOL, RegisterViewClass, (CRuntimeClass *pClass, const CString& strCName))					// ע��ɶ�̬������
	PUREMETHOD_(BOOL, RegisterMsgEntry, (PreTranslateMessageEntry pfnMsgEntry))								// ע��ɶ�̬������
	PUREMETHOD_(BOOL, RegisterDlgClass, (CRuntimeClass *pClass, const CString& strCName))					// ע��ɶ�̬�����Ի�����
END_GWINTERFACE()

typedef BOOL (*GWInitPlugin)(IPluginFactory* pPluginFactory);										// �����ʼ������

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
	FRAME_MENUBAR			= 0,				// �˵�
	FRAME_TOOLBAR			= 1,				// ������
	FRAME_STATUSBAR			= 2,				// ״̬��
	FRAME_USERPLUGIN		= 3					// �û��Զ�����
} GWFramePlugInType;

MIDL_INTERFACE("7186AC7C-E029-4f3e-811D-5DF892FE7615")
IFramePlugin : public IUnknown																							// ��ܲ��
{																							
public:
	STDMETHOD_(BOOL, CreatePlugin)(CWnd* pParent, UINT uID) PURE;														// ����
	STDMETHOD_(void, DestoryPlugin)() PURE;																				// ����
	STDMETHOD_(void, Reposition)(CRect& rcClient) PURE;																	// �ض�λ
	STDMETHOD_(GWFramePlugInType, GetPlugInType)() PURE;																// �������
	STDMETHOD_(BOOL, IsSupportDock)() PURE;																				// �Ƿ�֧�ָ���
	STDMETHOD_(BOOL, ProceedCommand)(const CString& strCommand) PURE;													// �����ݼ�����
	STDMETHOD_(void, ReLoad)() PURE;																					// ���¼��������ļ�
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