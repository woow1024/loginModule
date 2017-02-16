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
	// TT
	{"60" ,"TPME.XAGUSD" 		,4 ,8.27f ,10.27f},
	{"60" ,"TPME.PD" 				,3 ,6.27f ,8.27f},
	{"60" ,"TPME.PT" 				,6 ,8.27f ,10.27f},
	{"60" ,"TPME.NI" 				,5 ,8.27f ,10.27f},
	{"60" ,"TPME.CU" 				,5 ,8.27f ,10.27f},
	{"60" ,"TPME.AL" 				,5 ,8.27f ,10.27f},

	{"180" ,"TPME.XAGUSD"		,4 ,3.87f ,4.37f},
	{"180" ,"TPME.PD" 			,3 ,3.67f ,4.87f},
	{"180" ,"TPME.PT" 			,4 ,5.67f ,6.67f},
	{"180" ,"TPME.NI" 			,4 ,3.07f ,4.67f},
	{"180" ,"TPME.CU" 			,4 ,3.87f ,5.47f},
	{"180" ,"TPME.AL" 			,4 ,3.97f ,4.97f},

	{"240" ,"TPME.XAGUSD"		,4 ,3.07f ,4.27f},
	{"240" ,"TPME.PD" 			,3 ,3.27f ,4.87f},
	{"240" ,"TPME.PT" 			,4 ,5.27f ,7.27f},
	{"240" ,"TPME.NI" 			,4 ,2.57f ,3.67f},
	{"240" ,"TPME.CU" 			,4 ,4.87f ,6.47f},
	{"240" ,"TPME.AL" 			,4 ,3.27f ,4.07f},

	// BSY
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

	// SY
	{"60" ,"SZPEX.DSL0S" 		,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.GSL93S"		,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.JGTZJ" 		,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.QHO10S" 	,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.QHO20S" 	,4 ,2.57f ,3.57f},
	{"60" ,"SZPEX.QHO50S" 	,4 ,2.57f ,3.57f},

	{"180" ,"SZPEX.DSL0S" 		,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.GSL93S" 	,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.JGTZJ" 		,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.QHO10S"	,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.QHO20S"	,4 ,2.07f ,2.87f},
	{"180" ,"SZPEX.QHO50S"	,4 ,2.07f ,2.87f},

	{"240" ,"SZPEX.DSL0S" 		,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.GSL93S" 	,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.JGTZJ" 		,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.QHO10S"	,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.QHO20S"	,4 ,1.67f ,2.37f},
	{"240" ,"SZPEX.QHO50S"	,4 ,1.67f ,2.37f},

	// YG
	{"60" ,"PMEC.AG15"			,4,8.27f,10.27f},
	{"60" ,"PMEC.AG1"			,4,8.27f,10.27f},
	{"60" ,"PMEC.AG100"			,4,8.27f,10.27f},
	{"60" ,"PMEC.PTXA"			,6,8.27f,10.27f},
	{"60" ,"PMEC.PDXA"			,3,6.27f,8.27f},
	{"60" ,"PMEC.GDAG"			,4,8.27f,10.27f},
	{"60" ,"PMEC.GDPT"			,6,8.27f,10.27f},
	{"60" ,"PMEC.GDPD"			,3,6.27f,8.27f},

	{"180" ,"PMEC.AG15"			,4,3.87f,4.37f},
	{"180" ,"PMEC.AG1"			,4,3.87f,4.37f},
	{"180" ,"PMEC.AG100"		,4,3.87f,4.37f},
	{"180" ,"PMEC.PTXA"			,4,5.67f,6.67f},
	{"180" ,"PMEC.PDXA"			,3,3.67f,4.87f},
	{"180" ,"PMEC.GDAG"		,4,3.87f,4.37f},
	{"180" ,"PMEC.GDPT"			,4,5.67f,6.67f},
	{"180" ,"PMEC.GDPD"			,3,3.67f,4.87f},

	{"240" ,"PMEC.AG15"			,4,3.07f,4.27f},
	{"240" ,"PMEC.AG1"			,4,3.07f,4.27f},
	{"240" ,"PMEC.AG100"		,4,3.07f,4.27f},
	{"240" ,"PMEC.PTXA"			,4,5.27f,7.27f},
	{"240" ,"PMEC.PDXA"			,3,3.27f,4.87f},
	{"240" ,"PMEC.GDAG"		,4,3.07f,4.27f},
	{"240" ,"PMEC.GDPT"			,4,5.27f,7.27f},
	{"240" ,"PMEC.GDPD"			,3,3.27f,4.87f},
};