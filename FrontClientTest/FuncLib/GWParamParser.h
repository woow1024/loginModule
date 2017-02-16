#pragma once

#include <gwtypes.h>

#undef GW_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define GW_EXT_CLASS _declspec(dllexport)
#else
#define GW_EXT_CLASS _declspec(dllimport)
#endif

class GW_EXT_CLASS  GWParamParser
{
#define PARAM_COMMAND_SPLIT_CHAR		_T("?")
#define PARAM_ITEM_SPLIT_CHAR			_T(";")
#define PARAM_KEYVALUE_SPLIT_CHAR		_T("=")

DECLARE_MAP(CString, CString, CSTRING)

public:
	GWParamParser(void);
	explicit GWParamParser(const CString& strParamString);
	explicit GWParamParser(const GWParamParser& parser);

	~GWParamParser(void);

	BOOL Parse(const CString& strParamString);					// 解析参数字符串

	const CString& GetInputString() const { return m_strInputString; }
	const CString& GetCommandString() const { return m_strCommandString; }
	const CString& GetParamString() const { return m_strParamString; } 

	const CString ValueAsString(const CString& strKey, const CString& strDefault = _T("")) const;
	int ValueAsInt(const CString& strKey, int nDefault = 0) const;
	BOOL ValueAsBool(const CString& strKey, BOOL bDefault = TRUE) const;

	void RemoveKey(const CString& strKey);

	void AddKey(const CString& strKey, const CString& strValue);
	void AddKey(const CString& strKey, int  nValue);

	void ResetCommandString(const CString& strCommandString) { m_strCommandString = strCommandString; }
	void ReBuildInputString();

	BOOL HasKey(const CString& strKey) const { return FindKey(strKey) != m_mapParam.end(); }
private:
	CONST_CSTRINGMAPITR FindKey (const CString& strKey) const;
private:
	CString m_strInputString;				// 原始字符串
	CString m_strCommandString;				// 命令字符串
	CString m_strParamString;				// 参数字符串
	CSTRINGMAP	m_mapParam;					// 参数
};
