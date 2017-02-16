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
	{"60" ,"BPEX.ULP1" 		,4 ,2.87f ,3.87f},
	{"60" ,"BPEX.ULP2" 		,4 ,2.87f ,3.87f},
	{"60" ,"BPEX.ULP3" 		,4 ,2.87f ,3.87f},
	{"60" ,"BPEX.ULP4" 		,4 ,2.87f ,3.87f},

	{"180" ,"BPEX.ULP1"		,4 ,2.07f ,2.07f},
	{"180" ,"BPEX.ULP2" 		,4 ,2.07f ,2.07f},
	{"180" ,"BPEX.ULP3" 		,4 ,2.07f ,2.07f},
	{"180" ,"BPEX.ULP4" 		,4 ,2.07f ,2.07f},

	{"240" ,"BPEX.ULP1"		,4 ,1.87f ,2.07f},
	{"240" ,"BPEX.ULP2"		,4 ,1.87f ,2.07f},
	{"240" ,"BPEX.ULP3"		,4 ,1.87f ,2.07f},
	{"240" ,"BPEX.ULP4" 		,4 ,1.87f ,2.07f},
};