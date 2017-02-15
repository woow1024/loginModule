#pragma once

#include "StdAfx.h"
#include "md5.h"
#include "base64.h"

namespace GW_Encrypt
{
	GW_DLLEXPORT CString MD5Encrypt(const CString& strCleartext, CString &strCiphertext, long lLen)
	{
		CString strReturn;
		unsigned char szClearText[1024] = {0};
		CString strTmpClearText = strCleartext;
		memcpy(szClearText,strTmpClearText.GetBuffer(),strTmpClearText.GetLength());
		strTmpClearText.ReleaseBuffer();

		char szTempOut[1024] = {0};
		strReturn = yt::md5_string(szClearText,szTempOut,lLen);
		strCiphertext = szTempOut;
		return strReturn;
	}

	GW_DLLEXPORT CString SimpleMD5Key(const CString& strText)
	{
		CString strKey;

		MD5Encrypt(strText, strKey, strText.GetLength());

		return strKey;
	}

	GW_DLLEXPORT CString BASE64Encrypt(const CString &strCleartext,long lLen)
	{
		unsigned char szClearText[1024] = {0};
		CString strTmpClearText = strCleartext;
		memcpy(szClearText,strTmpClearText.GetBuffer(),strTmpClearText.GetLength());
		strTmpClearText.ReleaseBuffer();

		string strCiphertext;
		strCiphertext = base64_encode(szClearText , lLen);

		CString strReturn = strCiphertext.c_str();
		return strReturn;
	}

	GW_DLLEXPORT CString BASE64Decode(const CString &strCiphertext,long lLen)
	{
		CString strTmpCipherText = strCiphertext;

		string strCiphertext2 = strTmpCipherText.GetBuffer();
		strTmpCipherText.ReleaseBuffer();

		CString strReturn = base64_decode(strCiphertext2).c_str();
		return strReturn;
	}
};
