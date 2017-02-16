#pragma once

#include "GWDefine.h"

DECLARE_MAP(CString, CString, CSTRING)

#define KEY_SPLIT_CHAR		";"
#define VALUE_SPLIT_CHAR	"="
#define ITEM_SPLIT_CHAR		"|"

class stringsplitter
{
public:
	stringsplitter(const char* delimit = "\n")
	{
		setdelimit(delimit);
	}

	void setdelimit(const char* delimit)
	{
		this->delimit = delimit;
	}

	int split(const char* str)
	{
		result_vec.clear();

		if (str == NULL || strlen(str) == 0)
		{
			return 0;
		}

		int len = strlen(str) + 1;

		char *buffer = new char[len];
		memset(buffer, 0, len);
		strcpy_s(buffer, len, str);

		char *token = NULL;
		char *p = strtok_s(buffer, delimit, &token);

		while (p)
		{
			result_vec.push_back(p);
			p = strtok_s(NULL, delimit, &token);
		}

		delete []buffer;

		return size();
	}

	int size()
	{
		return result_vec.size();
	}

	CSTRINGVECITR begin()
	{
		return result_vec.begin();
	}

	CSTRINGVECITR end()
	{
		return result_vec.end();
	}

	const CString& operator[](int index)
	{
		if (index >= size())
		{
			static CString empty_string;
			return empty_string;
		}

		return result_vec[index];
	}

	CSTRINGVEC& getdata()
	{
		return result_vec;
	}
private:
	CString		delimit;				// 分隔符
	CSTRINGVEC  result_vec;				// 结果集
};
