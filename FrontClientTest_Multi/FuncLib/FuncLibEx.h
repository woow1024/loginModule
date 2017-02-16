#pragma once

#include "FuncLib.h"
#include <GWMessage.h>

#undef GW_FUNCLIB_API
#ifdef GWFUNCLIB_PROJECT  
#define GW_FUNCLIB_API					GW_DLLEXPORT
#else
#define GW_FUNCLIB_API					GW_DLLIMPORT
#endif

namespace GW_Msg
{
	// ·¢ËÍMSG_DIRECT_COMMAND
	GW_FUNCLIB_API GWRESULT SendDirectMessage(const CString& strMsgContent, CWnd* pSenderWnd = NULL, CWnd *pTargetWnd = NULL, BOOL bPost = TRUE);


}