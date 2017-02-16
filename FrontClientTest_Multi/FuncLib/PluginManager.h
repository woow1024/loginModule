#pragma once

#include <gwinterface.h>
#include <toolclass.h>

#undef GW_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define GW_EXT_CLASS GW_DLLEXPORT
#else
#define GW_EXT_CLASS GW_DLLIMPORT
#endif

class GW_EXT_CLASS CPluginManager : public IPluginFactory, public static_singletonbase<CPluginManager>
{
	IMPLEMENT_STATIC_SINGLETON_MODE(CPluginManager)
private:
	DECLARE_VECTOR(HMODULE, DLLMODULE)
	DECLARE_VECTOR(PreTranslateMessageEntry, MESSAGEENTY)

	typedef struct tagClassContext
	{
		CString strCNName;					// 类中文名
		CRuntimeClass* pRumtimeClass;		// RUNTIMECLASS
	} GWClassContext;

	DECLARE_MAP(CString, GWClassContext, CLASSCONTEXT)	// 创建信息

	DECLARE_MAP(CString, CWnd*, CLASSRECYCLE)			// 类回收站
public:
	DECLARE_VECTOR(CRuntimeClass*, FRAMEPLUGIN)
private:
	~CPluginManager(void);

	BOOL LoadPluginDll(const CString& strDllName);
	void CheckPluginDir();
public:
	virtual BOOL RegisterFramePlugin(CRuntimeClass *pClass);									// 注册框架插件
	virtual BOOL RegisterViewClass(CRuntimeClass *pClass, const CString& strCName);				// 注册可动态创建类
	virtual BOOL RegisterMsgEntry(PreTranslateMessageEntry pfnMsgEntry);						// 注册消息循环

	virtual BOOL RegisterDlgClass(CRuntimeClass *pClass, const CString& strCName);				// 注册可动态创建对话框类
public:
	BOOL LoadPlugin();
	void UnloadPlugin();

	static inline UINT GetNextID() { return m_sNextID++; }
public:
	void GetAllPluginViewName(std::vector<std::pair<CString, CString> >& pairViewInfo);

	BOOL PreTranslateMessage(MSG* pMsg);
	const FRAMEPLUGINVEC& GetFramePluginList() const { return m_vecFramePlugin; }

	CWnd* CreateViewByName(const CString& strClassName);
	void DestroyView(CWnd* pWnd, BOOL bNoCheckSingleton = FALSE);

	CRuntimeClass* GetClassRuntimeClass(const CString& strClassName);

	CWnd* CreateDlgByName(const CString& strClassName);
	void DestroyDlg(CWnd* pWnd, BOOL bNoCheckSingleton = FALSE);
	CRuntimeClass* GetDlgClassRuntimeClass(const CString& strClassName);
private:
	DLLMODULEVEC	m_vecModule;					// 模块

	MESSAGEENTYVEC	m_vecMsgEntry;					// 消息入口
	FRAMEPLUGINVEC  m_vecFramePlugin;				// 系统插件
	CLASSCONTEXTMAP m_mapViewClass;					// 视图插件
	CLASSCONTEXTMAP m_mapDlgClass;					// 对话框插件


	CLASSRECYCLEMAP	m_mapClassCycle;				// 类回收站

	static UINT		m_sNextID;						// 下一个ID
};

GW_EXT_CLASS CPluginManager* GetPluginManager();