#pragma once

#include <gwtypes.h>
#undef GW_FUNCLIB_API
#ifdef GWFUNCLIB_PROJECT  
#define GW_FUNCLIB_API					GW_DLLEXPORT
#else
#define GW_FUNCLIB_API					GW_DLLIMPORT
#endif


class GW_FUNCLIB_API MHSymbolRadar
{
public:
	MHSymbolRadar(LPCTSTR lpSymbolObj = NULL);
	void SetSymbol(CString strSymbol);
	BOOL IsTriggerCondition( GWSnapShotPack* pPack,RADARHISTORYVEC& radahisVec);

	void InitRadar();
	void InitRadarCons();
	void InitRadarHis();
	void InitRadarRing();

	BOOL FindRadarCon(CString strSymbol,CString strName,RADARCON& radars);
	RADARHISTORYVEC GetRadarsHis();

	static BOOL IsSymbolWarn(SymbolObj* pSymobj); 
	static BOOL InvalidSymbolWarn(SymbolObj* pSymobj);

	static void GetWarnSymbols(CSTRINGVEC& symVec);
	CString GetSymbolObj();
	BOOL IsActiveRadar();
	BOOL IsAlarmSound();

	BOOL SaveRadarsSetting(RADARCONTIONVEC& radarsVec);
	BOOL SaveRadarsHis(RADARHISTORYVEC& radarhisVec);
	BOOL SaveRadarRing(BOOL bRing);
private:
	BOOL IsTriggerLastPrice(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // 最新价
	BOOL IsTriggerBuy(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // 买入价
	BOOL IsTriggerSell(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // 卖出价
	BOOL IsTriggerRiseRange(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // 涨幅
	BOOL IsTriggerFallRange(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // 跌幅
	BOOL IsTriggerVolNow(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // 现量
protected:
	CString m_strSymbol;
	BOOL	m_bRing;				// 预警铃声
	RADARCONTIONVEC m_radarVec;
	RADARHISTORYVEC  m_radarHisVes;
} ;

DECLARE_VECTOR(MHSymbolRadar, MHSYMBOLCRADAR)