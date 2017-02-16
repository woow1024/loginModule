#include "stdafx.h"
#include "FuncLib.h"

namespace GW_Monitor
{

GWBOOL GetCurrentMonitorRect(CWnd* pWnd, CRect& rcMonitor, MonitorRectType eType)
{
	ASSERT(pWnd->GetSafeHwnd() != NULL);

	if (pWnd->GetSafeHwnd() == NULL)
	{
		return FALSE;
	}

	HMONITOR hMonitor = MonitorFromWindow(pWnd->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);

	ASSERT(hMonitor != NULL);

	MONITORINFO info = { sizeof(info) };
	GetMonitorInfo(hMonitor, &info);

	if (eType == EntireMonitor)
	{
		rcMonitor.CopyRect(&info.rcMonitor);
	}
	else
	{
		rcMonitor.CopyRect(&info.rcWork);
	}

	return TRUE;
}

}
