#include "StdAfx.h"
#include "PluginManager.h"
#include "GlobalSetting.h"
#include "Markup.h"
#include "FuncLib.h"

#include <algorithm>
using namespace std;

UINT CPluginManager::m_sNextID	= VIEWPLUGIN_BASE_ID;				

CPluginManager* GetPluginManager()
{
	return CPluginManager::GetInstance();
}

CPluginManager::~CPluginManager(void)
{
	UnloadPlugin();
}

BOOL CPluginManager::RegisterFramePlugin(CRuntimeClass *pClass)
{
	ASSERT(pClass != NULL);

	if (pClass == NULL)
	{
		return FALSE;
	}

	if (find(m_vecFramePlugin.begin(), m_vecFramePlugin.end(), pClass) != m_vecFramePlugin.end())
	{
		WriteLogMessage(LogWarning, _T("框架插件[%s]已存在"), pClass->m_lpszClassName);
	}
	else
	{
		m_vecFramePlugin.push_back(pClass);
	}
	
	return TRUE;
}

BOOL CPluginManager::RegisterViewClass(CRuntimeClass *pClass, const CString& strCName)
{
	ASSERT(pClass != NULL);

	if (pClass == NULL)
	{
		return FALSE;
	}

	if (m_mapViewClass.find(pClass->m_lpszClassName) != m_mapViewClass.end())
	{
		WriteLogMessage(LogWarning, _T("视图插件[%s]已存在"), pClass->m_lpszClassName);
	}
	else
	{
		GWClassContext contex;
		contex.strCNName = strCName;
		contex.pRumtimeClass = pClass;

		m_mapViewClass.insert(CLASSCONTEXTMAP::value_type(pClass->m_lpszClassName, contex));
	}

	return TRUE;
}

BOOL CPluginManager::RegisterMsgEntry(PreTranslateMessageEntry pfnMsgEntry)
{
	ASSERT(pfnMsgEntry != NULL);

	if (pfnMsgEntry == NULL)
	{
		return FALSE;
	}

	if (find(m_vecMsgEntry.begin(), m_vecMsgEntry.end(), pfnMsgEntry) != m_vecMsgEntry.end())
	{
		WriteLogMessage(LogWarning, _T("插件消息入口[%0X]已存在"), pfnMsgEntry);
	}
	else
	{
		m_vecMsgEntry.push_back(pfnMsgEntry);
	}
	
	return TRUE;
}

BOOL CPluginManager::RegisterDlgClass( CRuntimeClass *pClass, const CString& strCName )
{
	ASSERT(pClass != NULL);

	if (pClass == NULL)
	{
		return FALSE;
	}

	if (m_mapDlgClass.find(pClass->m_lpszClassName) != m_mapDlgClass.end())
	{
		WriteLogMessage(LogWarning, _T("对话框插件[%s]已存在"), pClass->m_lpszClassName);
	}
	else
	{
		GWClassContext contex;
		contex.strCNName = strCName;
		contex.pRumtimeClass = pClass;

		m_mapDlgClass.insert(CLASSCONTEXTMAP::value_type(pClass->m_lpszClassName, contex));
	}

	return TRUE;
}

BOOL CPluginManager::LoadPlugin()
{
	const CString PLUSINCFGFILENAME = _T("plugin.xml");
	CString strConfigFile = GlobalSetting::GetInstance()->GetGFSubDir(GDT_CFG);
	strConfigFile += PLUSINCFGFILENAME;

	CMarkup xmlDoc;

	if (!xmlDoc.Load(strConfigFile))
	{
		WriteLogMessage(LogError, xmlDoc.GetError());
		return FALSE;
	}

	xmlDoc.ResetMainPos();

	while (xmlDoc.FindChildElem(_T("plugin")))
	{
		xmlDoc.IntoElem();

		CString strDllName = xmlDoc.GetAttrib(_T("name"));

		LoadPluginDll(strDllName);

		xmlDoc.OutOfElem();
	}

	CheckPluginDir();

	return TRUE;
}

void CPluginManager::UnloadPlugin()
{
	m_vecFramePlugin.clear();
	m_vecMsgEntry.clear();
	m_mapViewClass.clear();
	m_mapDlgClass.clear();

	for_each(m_vecModule.begin(), m_vecModule.end(), FreeLibrary);
	m_vecModule.clear();
}

BOOL CPluginManager::LoadPluginDll(const CString& strDllName)
{
	HMODULE hModule = LoadLibrary(strDllName);
	
	if (hModule == NULL)
	{
		WriteLogMessage(LogError, _T("加载插件[%s]失败，GetLastError[%d]"), strDllName, GetLastError());		
		return FALSE;
	}
	
	if (find(m_vecModule.begin(), m_vecModule.end(), hModule) != m_vecModule.end())		// 重复加载同一个插件
	{
		WriteLogMessage(LogNormal, _T("插件[%s]被重复加载"), strDllName);
		FreeLibrary(hModule);
		return TRUE;
	}

	GWInitPlugin pInitFunc = (GWInitPlugin)GetProcAddress(hModule, PLUGIN_INITFUNC_NAME);
	
	if (pInitFunc == NULL)
	{
		WriteLogMessage(LogError, _T("加载插件[%s]失败,找不到GWInitPlugin函数入口"), strDllName);
		FreeLibrary(hModule);
		return FALSE;
	}
	
	if (!pInitFunc(this))
	{
		WriteLogMessage(LogWarning, _T("加载插件[%s]失败, GWInitPlugin函数返回值为FASLE"), strDllName);
		FreeLibrary(hModule);
		return FALSE;
	}
	
	m_vecModule.push_back(hModule);

	WriteDebugMessage(_T("加载插件[%s]成功"), strDllName);

	return TRUE;
}

void CPluginManager::CheckPluginDir()
{
	CString strDir = GlobalSetting::GetInstance()->GetGFSubDir(GDT_PLUGIN);

	CSTRINGVEC vecFile;
	GW_PathFile::FindFiles(strDir, vecFile, _T("*.dll"));

	for (CSTRINGVECITR itr = vecFile.begin(); itr != vecFile.end(); ++itr)
	{
		LoadPluginDll(*itr);
	}
}

BOOL CPluginManager::PreTranslateMessage(MSG* pMsg)
{
	for (CONST_MESSAGEENTYVECITR itr = m_vecMsgEntry.begin(); itr != m_vecMsgEntry.end(); ++itr)
	{
		PreTranslateMessageEntry pEntry = (PreTranslateMessageEntry)*itr;

		ASSERT(pEntry != NULL);

		if (pEntry == NULL)
		{
			continue;
		}

		if ((pEntry)(pMsg))
		{
			return TRUE;
		}
	}

	return FALSE;
}

CWnd* CPluginManager::CreateViewByName(const CString& strClassName)
{
	// 首先在回收站里找一下
	CLASSRECYCLEMAPITR itr = m_mapClassCycle.find(strClassName);

	if (itr != m_mapClassCycle.end())			// 回收站中有
	{
		CWnd * pView = itr->second;

		m_mapClassCycle.erase(itr);
		return pView;
	}
	else
	{
		CRuntimeClass* pRuntimeClass = GetClassRuntimeClass(strClassName);

		if (pRuntimeClass == NULL)
		{
			return NULL;
		}


		return dynamic_cast<CWnd*>(pRuntimeClass->CreateObject());
	}

	return NULL; 
}

void CPluginManager::DestroyView(CWnd* pWnd, BOOL bNoCheckSingleton)
{
	ASSERT(pWnd != NULL);

	if (!pWnd)
	{
		return;
	}

	if (bNoCheckSingleton)
	{
		pWnd->DestroyWindow();
		return;
	}

	IGWViewPlugin* pPlugin = GWGetInterface(pWnd, IGWViewPlugin);		

	BOOL bDoDestroy = TRUE;

	if (pPlugin && !pPlugin->PreDestroy())
	{
		bDoDestroy = FALSE;
	}

	if (!bDoDestroy)				// 视图阻止自己被销毁，加入回收站
	{
		CString strClassName = pWnd->GetRuntimeClass()->m_lpszClassName;

		m_mapClassCycle[strClassName] = pWnd;

		pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		pWnd->DestroyWindow();
	}
}

CRuntimeClass* CPluginManager::GetClassRuntimeClass(const CString& strClassName)
{
	CLASSCONTEXTMAPITR itr = m_mapViewClass.find(strClassName);

	if (itr == m_mapViewClass.end())
	{
		return NULL;
	}

	return itr->second.pRumtimeClass;
}

CRuntimeClass* CPluginManager::GetDlgClassRuntimeClass( const CString& strClassName )
{
	CLASSCONTEXTMAPITR itr = m_mapDlgClass.find(strClassName);

	if (itr == m_mapDlgClass.end())
	{
		return NULL;
	}

	return itr->second.pRumtimeClass;
}

CWnd* CPluginManager::CreateDlgByName( const CString& strClassName )
{

	// 首先在回收站里找一下
	CLASSRECYCLEMAPITR itr = m_mapClassCycle.find(strClassName);

	if (itr != m_mapClassCycle.end())			// 回收站中有
	{
		CWnd * pView = itr->second;

		m_mapClassCycle.erase(itr);
		return pView;
	}
	else
	{
		CRuntimeClass* pRuntimeClass = GetDlgClassRuntimeClass(strClassName);

		if (pRuntimeClass == NULL)
		{
			return NULL;
		}


		return dynamic_cast<CWnd*>(pRuntimeClass->CreateObject());
	}

	return NULL; 
}

void CPluginManager::DestroyDlg( CWnd* pWnd, BOOL bNoCheckSingleton /*= FALSE*/ )
{
	ASSERT(pWnd != NULL);

	if (!pWnd)
	{
		return;
	}

	CString strClassName = pWnd->GetRuntimeClass()->m_lpszClassName;

	if (bNoCheckSingleton)
	{
		pWnd->DestroyWindow();
		delete pWnd;

		return;
	}

	IGWDlgPlugin* pPlugin = GWGetInterface(pWnd, IGWDlgPlugin);		

	BOOL bDoDestroy = TRUE;

	if (pPlugin && !pPlugin->PreDestroy())
	{
		bDoDestroy = FALSE;
	}

	if (!bDoDestroy)				// 视图阻止自己被销毁，加入回收站
	{
		m_mapClassCycle[strClassName] = pWnd;

		pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		pWnd->DestroyWindow();
		delete pWnd;
	}
}

void CPluginManager::GetAllPluginViewName(std::vector<std::pair<CString, CString> >& pairViewInfo)
{
	for (CLASSCONTEXTMAPITR itr = m_mapViewClass.begin(); itr != m_mapViewClass.end(); ++itr)
	{
		pairViewInfo.push_back(std::pair<CString, CString>(itr->first, itr->second.strCNName));
	}
}
