#pragma once

#include <gwtypes.h>
#include <toolclass.h>

#undef _MX_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define _MX_EXT_CLASS _declspec(dllexport)
#else
#define _MX_EXT_CLASS _declspec(dllimport)
#endif

class _MX_EXT_CLASS CodeListMgr : public static_singletonbase<CodeListMgr>
{
	IMPLEMENT_STATIC_SINGLETON_MODE(CodeListMgr)
public:
	~CodeListMgr(void);
	
	void SetActiveSymbolObj(SymbolObj* pSymbolObj);
	void SetActiveMarket(MarketInfo* pMarket);
	void SetActiveMarket(const CString& strMarket);

	void SetActiveSymbolsVec(SYMBOLVEC& vecSymbols);

	SymbolObj*	GetActiveSymbolObj();
	MarketInfo* GetActiveMarket();

	void		AddSymbol(SymbolObj* pSymbolObj);												// 添加代码
	void		UpdateSymbol(SymbolObj* pSymbolObj);											// 更新代码
	
	void		MakeRelationship();																// 创建父子关系

	SymbolObj*	FindSymbolObj(const CString& strMarket, const CString &strCode);				// 查找代码
	
	void		AddMarket(MarketInfo* pMarket);													// 添加市场
	MarketInfo* FindMarket(const CString& strMarket);											// 查找市场
	
	void		DoMarketAdjust();																// 维护一下市场顺序

	void		GetAllMarket(MRKETVEC& vecName);												// 获取所有市场名称
	void		GetAllMarketNames(CSTRINGVEC& vecName);											// 获取所有市场
	void		GetMarketSymbols(const CString& strMarket, SYMBOLVEC& vecSymbol, BOOL bOnlyTopLevelSymbol);				// 获取指定市场下所有代码

	void		KeyworkFilter(const CString& strText, WIZZARDOBJLST& lstMatch, DWORD dwMask);	// 获取匹配对象

	BOOL		LoadLocalConfig();																// 加载本地配置文件

	BOOL		AddSelfStock(SymbolObj* pSymbolObj);											// 增加自选股
	BOOL        ReplaceSelfStock(SYMBOLVEC vecSymbol);                                          // 替换掉当前的自选股
	BOOL		RemoveSelfStock(SymbolObj* pSymbolObj);											// 删除自选股
	BOOL		MoveSelfStockPostion(SymbolObj* pSymbolObj, BOOL bMoveDown);					// 移动自选股位置，上移或下移一位 
	void		SaveSelfStockToFile();															// 保持自选股文件到配置文件

	SymbolObj*	GetNextSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket);				// 获取下一只股票
	SymbolObj*	GetPreviousSymbolObj(SymbolObj* pSymbolObj, const CString& strMarket);			// 获取上一只股票
	
	void MakeSymbolRight(GWSYMBOLRIGHTVEC& vecSymbolRight);		//关联 SLSP权限品种
	void GetMarketSymbolLoopVector(SYMBOLVEC& vecMarketSymbol, const CString& strMarket, SymbolObj* pStartSymbolObj, UINT nMaxCount, BOOL bLoop);		 // 获取选中市场的股票循环队列
private:
	const CString GetSysShortcutFileName();														// 获取系统快捷键设置文件名称
	const CString GetUserShortcutFileName();													// 获取系统快捷键设置文件名称

	const CString GetSelfStockFileName();														// 获取自选股文件名称

	SYMBOLVEC&  GetMarketSymbolVec(const CString& strMarketName);								// 获取市场商品队列

	void		CreateSelfStockMarket();														// 创建自选股市场
		
	BOOL		LoadSelfStockFromFile();														// 从文件自选股
	BOOL		LoadShortcutFromFile(const CString& strFileName);								// 从文件加载

	BOOL		CheckMarketValid(MarketInfo* pMarket);											// 检查市场是否有效

	void		InitInstance();
private:
	MARKETMAP			m_mapMarket;				// 市场代码表

	WIZZARDOBJLST		m_lstShortcutObj;			// 快捷键对象列表
	WIZZARDOBJLST		m_lstSymbolObj;				// 码表对象列表
	MARKETLST			m_lstMarket;				// 市场对象列表，保持一份原始顺序的市场

	SymbolObj*			m_pActiveSymbol;			// 当前symbol
	MarketInfo*			m_pActiveMarket;			// 当期Market

	CSTRINGVEC			m_vecOmitMarket;			// 需要忽略的市场

	SYMBOLVEC			m_vecActiveSymbols;
};

_MX_EXT_CLASS CodeListMgr* GetCodeListMgr();