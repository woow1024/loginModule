#include "stdafx.h"

#include <GWDefine.h>
#include <gwtypes.h>

#include <io.h>
#include "Markup.h"

namespace GW_PathFile
{
const TCHAR DRIVE_IDENTIFICATION = _T(':');					// 盘符标识符
const TCHAR PATH_SPLIT_CHAR_W	= _T('\\');					// window风格路径分割符
const TCHAR PATH_SPLIT_CHAR_U	= _T('/');					// LINUX风格路径分割符

GW_DLLEXPORT GWBOOL IsPathFileExist(const CString& strPathFile)
{
	return _taccess(strPathFile, 0) == 0;
}

GW_DLLEXPORT GWBOOL IsNetPath(const CString& strPath)
{
	if(strPath.GetLength() <= 2)
	{
		return FALSE;
	}

	// 长度大于2才有比较价值
	if (strPath.GetAt(0) == PATH_SPLIT_CHAR_W && strPath.GetAt(1) == PATH_SPLIT_CHAR_W)
	{
		return TRUE;
	}

	return FALSE;
}

GW_DLLEXPORT GWBOOL IsRelativePath(const CString& strPath)
{
	if (IsNetPath(strPath))		// 是网络路径
	{
		return FALSE;
	}

	if(strPath.GetLength() <= 2)	// 长度不够，肯定是相对路径
	{
		return TRUE;
	}

	if (strPath.GetAt(1) == DRIVE_IDENTIFICATION)
	{
		return FALSE;
	}

	return TRUE;
}
								
GW_DLLEXPORT const CString GetParentPath(const CString& strFileName)					
{
	CString strPath = strFileName;

	strPath.TrimRight(PATH_SPLIT_CHAR_W);
	strPath.TrimRight(PATH_SPLIT_CHAR_U);

	TCHAR szDirve[8], szPath[256], szFileName[256], szExtName[64];

	_tsplitpath_s(strPath, szDirve, szPath, szFileName, szExtName);

	strPath.Format(_T("%s%s"), szDirve, szPath);

	return strPath;
}


GW_DLLEXPORT CString FullPath2ShortPath(const CString& strFullPahtFileName)
{
	TCHAR szShortPaht[256];
	ZeroMemory(szShortPaht, sizeof(szShortPaht));
	::GetShortPathName((LPCTSTR)strFullPahtFileName, szShortPaht, 255);
	CString strShortPath = szShortPaht;
	return strShortPath;
}

GW_DLLEXPORT GWBOOL CreateDir(const CString& strPath)
{
	if (IsPathFileExist(strPath))
	{
		return TRUE;
	}

	TCHAR szBuffer[1024];

	if (IsRelativePath(strPath))			// 相对路径转化为绝对路径
	{
		GetCurrentDirectory(_countof(szBuffer), szBuffer);
		_tcscat_s(szBuffer, _T("\\"));
		_tcscat_s(szBuffer, strPath);

		return CreateDir(szBuffer);
	}

	CString strParentPath = GetParentPath(strPath);

	if (CreateDir(strParentPath))
	{
		return CreateDirectory(strPath, NULL);
	}
	else
	{
		return FALSE;
	}	

	return TRUE;
}

GW_DLLEXPORT GWBOOL EnsureDirExist(const CString& strPath)	
{
	return CreateDir(strPath);
}

GW_DLLEXPORT GWUINT FindFiles(const CString& strDir, CSTRINGVEC& vecFile, const CString& strExtName = _T("*.*"))
{
	CString strFilter = strDir;

	strFilter.TrimRight(PATH_SPLIT_CHAR_W);
	strFilter.TrimRight(PATH_SPLIT_CHAR_U);

	strFilter += "\\" + strExtName;

	CFileFind clsFind;

	BOOL bFind = clsFind.FindFile(strFilter);

	while (bFind)
	{
		bFind = clsFind.FindNextFile();
		vecFile.push_back(clsFind.GetFilePath());
	}

	clsFind.Close();

	return vecFile.size();
}

GW_DLLEXPORT GWBOOL DeleteFile(const CString& strFile)
{
	if(FALSE == GW_PathFile::IsPathFileExist(strFile))
	{
		return TRUE;
	}

	BOOL bRet = ::DeleteFile(strFile);

	if (!bRet && GetLastError() == ERROR_ACCESS_DENIED)
	{
		SetFileAttributes(strFile, GetFileAttributes(strFile) & (~FILE_ATTRIBUTE_READONLY));
		bRet = ::DeleteFile(strFile);
	}

	return bRet;
}

GW_DLLEXPORT GWBOOL DeleteDir(const CString&  strDir)
{
	if (!IsPathFileExist(strDir))
	{
		return TRUE;
	}

	CString strFilter = strDir;

	strFilter.TrimRight(PATH_SPLIT_CHAR_W);
	strFilter.TrimRight(PATH_SPLIT_CHAR_U);
	
	strFilter += "\\*.*";

	CFileFind clsFind;

	BOOL bFind = clsFind.FindFile(strFilter);
	
	while (bFind)
	{
		bFind = clsFind.FindNextFile();
		
		if (clsFind.IsDots())
		{
			continue;
		}

		CString strFileName = clsFind.GetFilePath();

		if (clsFind.IsDirectory())
		{
			DeleteDir(strFileName);
		}
		else
		{
			DeleteFile(strFileName);
		}
	}

	clsFind.Close();

	return RemoveDirectory(strDir);
}

GW_DLLEXPORT GWBOOL CreateEmptyXMLFile( const CString& strFileName )
{
	BOOL bRes = FALSE;
	if (strFileName.IsEmpty() == FALSE)
	{
		CMarkup xmlParam;
		try
		{
			xmlParam.SetDoc(_T("<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n"));

			xmlParam.AddElem(_T("ROOT"));
			bRes = xmlParam.Save(strFileName);
		}
		catch(...)
		{
			bRes = FALSE;
		}
	}

	return bRes;
}



}
