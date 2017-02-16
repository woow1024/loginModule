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
	BOOL IsTriggerLastPrice(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // ���¼�
	BOOL IsTriggerBuy(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // �����
	BOOL IsTriggerSell(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // ������
	BOOL IsTriggerRiseRange(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // �Ƿ�
	BOOL IsTriggerFallRange(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // ����
	BOOL IsTriggerVolNow(RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack); // ����
protected:
	CString m_strSymbol;
	BOOL	m_bRing;				// Ԥ������
	RADARCONTIONVEC m_radarVec;
	RADARHISTORYVEC  m_radarHisVes;
} ;

DECLARE_VECTOR(MHSymbolRadar, MHSYMBOLCRADAR)