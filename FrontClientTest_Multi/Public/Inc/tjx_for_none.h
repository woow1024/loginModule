#pragma once

#define TJX_MAX_PARAM_COUNT   3
typedef struct tagTJX_Param
{
	CString strPeriod;
	CString strSymObj;
	float	  fParam[TJX_MAX_PARAM_COUNT];
}TJX_PARAM;

const BOOL g_tjx_default_Inited = FALSE;
const TJX_PARAM  TJX_PARAM_DEFAULT[]=
{
	{},
};