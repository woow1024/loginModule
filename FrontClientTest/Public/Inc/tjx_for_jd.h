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
	{"60" ,"SGE.AUT+D" 		,4 ,8.27f ,10.27f},
	{"60" ,"SGE.MAUT+D" 	,4 ,8.27f ,10.27f},
	{"60" ,"SGE.AGT+D" 		,4 ,8.27f ,10.27f},
	{"60" ,"INAU.XAU" 		,4 ,8.27f ,10.27f},
	{"60" ,"INAU.XAG" 		,4 ,8.27f ,10.27f},

	{"180" ,"SGE.AUT+D" 	,4 ,3.87f ,4.37f},
	{"180" ,"SGE.MAUT+D" 	,4 ,3.87f ,4.37f},
	{"180" ,"SGE.AGT+D" 	,4 ,3.87f ,4.37f},
	{"180" ,"INAU.XAU" 		,4 ,3.87f ,4.37f},
	{"180" ,"INAU.XAG" 		,4 ,3.87f ,4.37f},

	{"240" ,"SGE.AUT+D" 	,4 ,3.07f ,4.27f},
	{"240" ,"SGE.MAUT+D" 	,4 ,3.07f ,4.27f},
	{"240" ,"SGE.AGT+D" 	,4 ,3.07f ,4.27f},
	{"240" ,"INAU.XAU" 		,4 ,3.07f ,4.27f},
	{"240" ,"INAU.XAG" 		,4 ,3.07f ,4.27f},

};