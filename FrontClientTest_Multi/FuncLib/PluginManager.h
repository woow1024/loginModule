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
		CString strCNName;					// ��������
		CRuntimeClass* pRumtimeClass;		// RUNTIMECLASS
	} GWClassContext;

	DECLARE_MAP(CString, GWClassContext, CLASSCONTEXT)	// ������Ϣ

	DECLARE_MAP(CString, CWnd*, CLASSRECYCLE)			// �����վ
public:
	DECLARE_VECTOR(CRuntimeClass*, FRAMEPLUGIN)
private:
	~CPluginManager(void);

	BOOL LoadPluginDll(const CString& strDllName);
	void CheckPluginDir();
public:
	virtual BOOL RegisterFramePlugin(CRuntimeClass *pClass);									// ע���ܲ��
	virtual BOOL RegisterViewClass(CRuntimeClass *pClass, const CString& strCName);				// ע��ɶ�̬������
	virtual BOOL RegisterMsgEntry(PreTranslateMessageEntry pfnMsgEntry);						// ע����Ϣѭ��

	virtual BOOL RegisterDlgClass(CRuntimeClass *pClass, const CString& strCName);				// ע��ɶ�̬�����Ի�����
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
	DLLMODULEVEC	m_vecModule;					// ģ��

	MESSAGEENTYVEC	m_vecMsgEntry;					// ��Ϣ���
	FRAMEPLUGINVEC  m_vecFramePlugin;				// ϵͳ���
	CLASSCONTEXTMAP m_mapViewClass;					// ��ͼ���
	CLASSCONTEXTMAP m_mapDlgClass;					// �Ի�����


	CLASSRECYCLEMAP	m_mapClassCycle;				// �����վ

	static UINT		m_sNextID;						// ��һ��ID
};

GW_EXT_CLASS CPluginManager* GetPluginManager();