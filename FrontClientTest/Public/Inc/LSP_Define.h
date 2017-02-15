#pragma once

DECLARE_VECTOR(CString, CSTRING)

namespace LSP_PARAM
{
#if defined(VERSION_TAG_TT)
	//old
	const CString LSP_SYMBOL_CODE[] =  {_T("JG.267"),_T("JG.283"),_T("JG.281")};
	const CString LSP_SYMBOL_CODE1[] = {_T("JG.267"),_T("JG.283"),_T("JG.281"),_T("JG.284"),_T("JG.282"),_T("JG.265"),_T("JG.266"),_T("JG.268"),_T("JG.263"),_T("JG.264"),_T("JG.271"),_T("JG.272"),_T("JG.285"),_T("JG.286"),
	_T("PMEC.GDAG"),_T("PMEC.GDAGV"),_T("PMEC.AG15"),_T("PMEC.AG1"),_T("PMEC.AG100"),_T("PMEC.PDXA"),_T("PMEC.GDPD"),_T("PMEC.GDPT"),_T("PMEC.PTXA")};
	const CString LSP_SYMBOL_NAME[] = {_T("银"),_T("铂"),_T("钯")};
	const CString LSP_POSITIONS_NAME[] = {_T("EIA原油库存"),_T("黄金ETF持仓"),_T("白银ETF持仓")};

	//new 权限移植--huangxilong 2015-10-10
// 	const CString LSP_SYMBOL_CODE[] =  {_T("TPME.XAGUSD"),_T("TPME.NI"),_T("TPME.CU"),_T("TPME.AL"),_T("TPME.PT"),_T("TPME.PD"),_T("PMEC.GDAGV"),_T("PMEC.GDAG"),_T("PMEC.GDPD"),_T("PMEC.GDPT")};
// 	const CString LSP_SYMBOL_CODE1[] = {_T("TPME.XAGUSD"),_T("TPME.NI"),_T("TPME.CU"),_T("TPME.AL"),_T("TPME.PT"),_T("TPME.PD"),_T("PMEC.GDAGV"),_T("PMEC.GDAG"),_T("PMEC.GDPD"),_T("PMEC.GDPT")};
// 	const CString LSP_SYMBOL_NAME[] = {_T("银"),_T("镍"),_T("铜"),_T("铝"),_T("铂"),_T("钯"),_T("粤贵银锭"),_T("粤贵银"),_T("粤贵钯"),_T("粤贵铂")};
// 	const CString LSP_POSITIONS_NAME[] = {_T("黄金ETF持仓"),_T("白银ETF持仓"),_T("EIA原油库存")};
#elif defined(VERSION_TAG_YG) 
	const CString LSP_SYMBOL_CODE[] =  {_T("PMEC.GDAG"),_T("PMEC.GDPD"),_T("PMEC.GDPT")};

	const CString LSP_SYMBOL_CODE1[] = 
	{_T("PMEC.GDAG"),_T("PMEC.GDAGV"),_T("JG.267"),_T("JG.268"),_T("JG.263"),_T("JG.264"),_T("JG.265"),_T("JG.266"),
	
	_T("PMEC.GDPT"),_T("JG.271"),_T("JG.272"),_T("JG.283"),_T("JG.284"),
	
	_T("PMEC.GDPD"),_T("JG.285"),_T("JG.286"),_T("JG.281"),_T("JG.282")
	};

	const CString LSP_SYMBOL_NAME[] = {_T("银"),_T("钯"),_T("铂")};
	const CString LSP_POSITIONS_NAME[] = {_T("黄金ETF持仓"),_T("白银ETF持仓")};
#elif defined(VERSION_TAG_TD)
	const CString LSP_SYMBOL_CODE[] =  {_T("PMEC.GDAG")};
	const CString LSP_SYMBOL_CODE1[] = {_T("PMEC.GDAG")};
	const CString LSP_SYMBOL_NAME[] = {_T("白银")};
	const CString LSP_POSITIONS_NAME[] = {_T("黄金ETF持仓"),_T("白银ETF持仓")};
#elif defined(VERSION_TAG_BSY) || defined(VERSION_TAG_JD)
	const CString LSP_SYMBOL_CODE[] = {_T("BPEX.ULP1")};
	const CString LSP_SYMBOL_CODE1[] = {_T("BPEX.ULP1"),_T("BPEX.ULP2"),_T("BPEX.ULP3"),_T("BPEX.ULP4")};
	const CString LSP_SYMBOL_NAME[] = {_T("石油")};
	const CString LSP_POSITIONS_NAME[] = {_T("EIA原油库存")};
#elif defined(VERSION_TAG_SY)
	const CString LSP_SYMBOL_CODE[] =  {_T("SZPEX.DSL0S")};
	const CString LSP_SYMBOL_CODE1[] = {_T("SZPEX.DSL0S"),_T("SZPEX.GSL93S"),_T("SZPEX.JGTZJ"),_T("SZPEX.QHO10S"),_T("SZPEX.QHO20S"),_T("SZPEX.QHO50S")};
	const CString LSP_SYMBOL_NAME[] =  {_T("石油")};
	const CString LSP_POSITIONS_NAME[] = {_T("EIA原油库存")};
#elif defined(VERSION_TAG_ZY)
	const CString LSP_SYMBOL_CODE[] =  {_T("TPME.XAGUSD")};
	const CString LSP_SYMBOL_CODE1[] = {_T("TPME.XAGUSD"),_T("TPME.NI"),_T("TPME.CU"),_T("TPME.AL"),_T("TPME.PT"),_T("TPME.PD"),
	_T("PMEC.GDAG"),_T("PMEC.GDPD"),_T("PMEC.GDPT"),
	_T("PMEC.GDAG"),
	_T("BPEX.ULP1"),_T("BPEX.ULP2"),_T("BPEX.ULP3"),_T("BPEX.ULP4"),
	_T("SZPEX.DSL0S"),_T("SZPEX.GSL93S"),_T("SZPEX.JGTZJ"),_T("SZPEX.QHO10S"),_T("SZPEX.QHO20S"),_T("SZPEX.QHO50S"),};
	const CString LSP_SYMBOL_NAME[] =  {_T("白银")};
	const CString LSP_POSITIONS_NAME[] = {_T("白银ETF持仓")};
#elif defined(VERSION_TAG_DX)
	const CString LSP_SYMBOL_CODE[] =  {_T("PMEC.GDAG")};
	const CString LSP_SYMBOL_CODE1[] = {_T("PMEC.GDAG")};
	const CString LSP_SYMBOL_NAME[] = {_T("白银")};
	const CString LSP_POSITIONS_NAME[] = {_T("黄金ETF持仓"),_T("白银ETF持仓")};
#else
	const CString LSP_SYMBOL_CODE[] = {_T("")};
	const CString LSP_SYMBOL_CODE1[] = {_T("")};
	const CString LSP_SYMBOL_NAME[] = {_T("")};
	const CString LSP_POSITIONS_NAME[] = {_T("黄金ETF持仓"),_T("白银ETF持仓")};
#endif


	const CSTRINGVEC LSPGetAllSymbolCods()
	{
		CSTRINGVEC codeVec;
		for (int i = 0; i < sizeof(LSP_SYMBOL_CODE)/ sizeof(CString); i++)
		{
			if (LSP_SYMBOL_CODE[i] != _T(""))
			{
				codeVec.push_back(LSP_SYMBOL_CODE[i]);
			}
		}
		return codeVec;
	}

	const CSTRINGVEC LSPGetAllSymbolNames()
	{
		CSTRINGVEC nameVec;
		for (int i = 0; i < sizeof(LSP_SYMBOL_NAME)/ sizeof(CString); i++)
		{
			if (LSP_SYMBOL_NAME[i] != _T(""))
			{
				nameVec.push_back(LSP_SYMBOL_NAME[i]);
			}
		}
		return nameVec;
	}

	const CSTRINGVEC LSPGetAllPositions()
	{
		CSTRINGVEC positionsVec;
		for (int i = 0; i < sizeof(LSP_POSITIONS_NAME)/ sizeof(CString); i++)
		{
			if (LSP_POSITIONS_NAME[i] != _T(""))
			{
				positionsVec.push_back(LSP_POSITIONS_NAME[i]);
			}
		}
		return positionsVec;
	}

	BOOL IsSymbolHasLSPData(CString strSymbolObj)
	{
		if (strSymbolObj == _T(""))
			return FALSE;

		for (int i = 0; i < sizeof(LSP_SYMBOL_CODE1)/ sizeof(CString); i++)
		{
			if (LSP_SYMBOL_CODE1[i] == _T(""))
				continue;

			if (LSP_SYMBOL_CODE1[i].CompareNoCase(strSymbolObj) == 0)
			{
				return TRUE;
			}
		}
		return FALSE;
	}
}
