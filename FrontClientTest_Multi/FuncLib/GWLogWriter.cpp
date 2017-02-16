#include "StdAfx.h"
#include "GWLogWriter.h"
#include "GlobalSetting.h"

namespace GW_Log
{
#ifdef _DEBUG
	GW_DLLEXPORT GWVOID WriteDebugMessageImp(LPCTSTR lpFile, int nLine, GWLogLevel nLevel, LPCTSTR lpMessage, ...)
#else
	GW_DLLEXPORT GWVOID WriteDebugMessageImp(GWLogLevel nLevel, LPCTSTR lpMessage, ...)
#endif
	{
		if(NULL == lpMessage || 0 == _tcslen(lpMessage))
		{
			return;
		}
		if (!GWLogWriter::GetInstance()->CheckLogLevel(nLevel))
		{
			return;
		}

		CString strFormat, strMsg;

		time_t time_now = time(NULL);
		struct tm tm_now;
		localtime_s(&tm_now, &time_now);
		
		strFormat.Format(
				_T("%02d:%02d:%02d : %s"),
				tm_now.tm_hour,
				tm_now.tm_min,
				tm_now.tm_sec,
				lpMessage
				);

		va_list arg_ptr;
		va_start(arg_ptr, lpMessage);

		strMsg.FormatV(strFormat, arg_ptr);

		GWLogWriter::GetInstance()->WriteLog(strMsg);

#ifdef _DEBUG
			CString strDebugMsg;
			strDebugMsg.Format("%s(%d) : %s\n", lpFile, nLine, strMsg);
			OutputDebugString(strDebugMsg);
#endif
	}

	GW_DLLEXPORT const CString& GetLastLogFileName()
	{
		return GWLogWriter::GetInstance()->GetLastLogFileName();
	}
}

GWLogWriter::~GWLogWriter(void)
{
	m_logWriter.close();
}

void GWLogWriter::InitInstance()
{
	CString strLogFileName = GetGloablSetting()->GetGFSubDir(GDT_LOG) + _T("client.log");

	m_nLogLevel = (GWLogLevel)GetPrivateProfileInt(_T("system"), _T("log"), LogNormal, GetGloablSetting()->GetUserConfigFileName());

	if (m_nLogLevel != LogDisabled)
	{
		m_logWriter.init(strLogFileName, false, 1024 * 512, true);
	}
}

void GWLogWriter::WriteLog(const CString& strMessage)
{
	if(-1 != strMessage.Find('%'))
	{//% �滻Ϊ #
		CString strTemp = strMessage;
		strTemp.Replace('%', '#');
		m_logWriter.write(strTemp);
	}
	else
	{
		m_logWriter.write(strMessage);
	}
}


