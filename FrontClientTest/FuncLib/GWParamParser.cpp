#include "StdAfx.h"

#include <toolclass.h>

#include "FuncLib.h"
#include "GWParamParser.h"

GWParamParser::GWParamParser(void)
{
}

GWParamParser::GWParamParser(const CString& strParamString)
{
	Parse(strParamString);
}

GWParamParser::GWParamParser(const GWParamParser& parser)
{
	m_strInputString   = parser.GetInputString();
	m_strCommandString = parser.GetCommandString();
	m_strParamString   = parser.GetParamString();

	m_mapParam.insert(parser.m_mapParam.begin(), parser.m_mapParam.end());
}

GWParamParser::~GWParamParser(void)
{
}

BOOL GWParamParser::Parse(const CString& strParamString)
{
	if (!m_mapParam.empty())
	{
		m_strParamString.Empty();
		m_strCommandString.Empty();

		m_mapParam.clear();
	}

	m_strInputString = strParamString;		// ����ԭʼ�ַ���

	if (m_strInputString.IsEmpty())
	{
		return FALSE;
	}

	int nCommandSpPos  = m_strInputString.Find(PARAM_COMMAND_SPLIT_CHAR);			// ��һ�������ֵ�λ��
	int nKeyValueSpPos = m_strInputString.Find(PARAM_KEYVALUE_SPLIT_CHAR);			// ��һ��=���ֵ�λ��
// �����������=���棬Ӧ����û��command��ֻ��key-value
// �����������=ǰ�棬ȥ��ǰ����ַ���Ϊcommand���������������key-value

	if (nCommandSpPos == -1 && nKeyValueSpPos == -1)	// ��û�У�Ҳû��=��ȫ���ַ�����Ϊcommand
	{
		m_strCommandString = m_strInputString;
		return TRUE;
	}
	else if (nCommandSpPos == -1)			// û�У�ֻ��=��ȫ���ַ���������key-value
	{
		m_strParamString = m_strInputString;
	}
	else if (nKeyValueSpPos == -1)			// û��=ֻ��?		
	{
		m_strCommandString = m_strInputString.Left(nCommandSpPos);
		m_strParamString = m_strInputString.Mid(nCommandSpPos + 1);
	}
	else if (nCommandSpPos < nKeyValueSpPos)		// �����������������=ǰ��
	{
		m_strCommandString = m_strInputString.Left(nCommandSpPos);
		m_strParamString = m_strInputString.Mid(nCommandSpPos + 1);
	}
	else if (nKeyValueSpPos < nCommandSpPos) // ��������=����
	{
		m_strParamString = m_strInputString;
	}

	if (m_strParamString.IsEmpty())
	{
		return TRUE;
	}

	stringsplitter spItems(PARAM_ITEM_SPLIT_CHAR);
	spItems.split(m_strParamString);

	for (CSTRINGVECITR itr = spItems.begin(); itr != spItems.end(); ++itr)
	{
		stringsplitter spParam(PARAM_KEYVALUE_SPLIT_CHAR);					// ����ָ������⣬��������к���PARAM_KEYVALUE_SPLIT_CHAR��=��,�ᵼ�·ָ�����Ԥ�ڲ�������Ҫ�Ľ�
		spParam.split(*itr);

		ASSERT(spParam.size() >= 1);

		if (spParam.size() <= 0)
		{
			WriteDebugMessage("�޷����������ַ���[%s]", *itr);
			return FALSE;
		}

		CString strKey = spParam[0];

		if (spParam.size() == 2)
		{
			m_mapParam[strKey.MakeUpper()] = spParam[1];				// ���ｫkeyת��Ϊ��д
		}
		else if (spParam.size() == 1)
		{
			m_mapParam[strKey.MakeUpper()] = _T("");					// ʹ��һ���յ�valueֵ��Ӧkey
		}
		else
		{
			ASSERT(FALSE);
			WriteDebugMessage("�޷����������ַ���[%s]", *itr);
			return FALSE;
		}
	}

	return TRUE;
}

const CString GWParamParser::ValueAsString(const CString& strKey, const CString& strDefault) const
{
	CONST_CSTRINGMAPITR itr = FindKey(strKey);

	if (itr != m_mapParam.end())
	{
		return itr->second;
	}

	return strDefault;
}

CONST_CSTRINGMAPITR GWParamParser::FindKey(const CString& strKey)  const 
{
	CString strBuffer = strKey;
	return m_mapParam.find(strBuffer.MakeUpper());
}

int GWParamParser::ValueAsInt(const CString& strKey, int nDefault) const
{
	CString strDefault;
	strDefault.Format(_T("%d"), nDefault);

	return atol(ValueAsString(strKey, strDefault));
}

BOOL GWParamParser::ValueAsBool(const CString& strKey, BOOL bDefault) const
{
	const CString strBOOL[] = { _T("true"), _T("false") };

	CString strDefault;
	strDefault.Format(_T("%s"), strBOOL[!bDefault]);

	CString strValue = ValueAsString(strKey, strDefault);

	if (strValue.CompareNoCase("false") == 0 || strValue.CompareNoCase("0") == 0)
	{
		return FALSE;
	}

	return TRUE;
}

void GWParamParser::RemoveKey(const CString& strKey)
{
	CONST_CSTRINGMAPITR itr = FindKey(strKey);

	if (itr != m_mapParam.end())
	{
		m_mapParam.erase(itr);
	}
}

void GWParamParser::ReBuildInputString()
{
	CString strParamString;

	for (CSTRINGMAPITR itr = m_mapParam.begin(); itr != m_mapParam.end(); ++itr)
	{
		CString strParam;
		strParam.Format("%s=%s;", itr->first, itr->second);
		strParamString += strParam;
	}

	m_strParamString = strParamString.TrimRight(";");
	
	if (m_strCommandString.IsEmpty())
	{
		m_strInputString = m_strParamString;
	}
	else
	{
		if (m_strParamString.IsEmpty())
		{
			m_strInputString = m_strCommandString;
		}
		else
		{
			m_strInputString.Format("%s?%s", m_strCommandString, m_strParamString);
		}
	}
}

void GWParamParser::AddKey(const CString& strKey, const CString& strValue)
{
	CString strMapKey = strKey;
	strMapKey.MakeUpper();

	m_mapParam[strMapKey] = strValue;
}

void GWParamParser::AddKey(const CString& strKey, int nValue)
{
	CString strValue;
	strValue.Format(_T("%d"), nValue);

	AddKey(strKey, strValue);
}

