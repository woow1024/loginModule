#pragma once

#define TJX_MAX_PARAM_COUNT   3
typedef struct tagTJX_Param
{
	CString strPeriod;
	CString strSymObj;
	float	  fParam[TJX_MAX_PARAM_COUNT];
}TJX_PARAM;

const BOOL g_tjx_default_Inited = TRUE;
const TJX_PARAM  TJX_PARAM_DEFAULT[]=
{
	{"60" ,"SZPEX.DSL0S" 		,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.GSL93S"		,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.JGTZJ" 		,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.QHO10S" 	,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.QHO20S" 	,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.QHO50S" 	,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.SYSG100S" 	,4 ,2.57f ,3.57f},

	{"180" ,"SZPEX.DSL0S" 		,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.GSL93S" 	,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.JGTZJ" 		,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.QHO10S"	,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.QHO20S"	,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.QHO50S"	,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.SYSG100S"	,4 ,2.07f ,2.87f},

	{"240" ,"SZPEX.DSL0S" 		,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.GSL93S" 	,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.JGTZJ" 		,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.QHO10S"	,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.QHO20S"	,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.QHO50S"	,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.SYSG100S"	,4 ,1.67f ,2.37f},
};