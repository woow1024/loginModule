#include "string_splitter.h"
#include <string.h>

static const char default_split_char[] = "\n";

string_splitter::string_splitter(const char* split_char) : splite_once(false)
{
	set_split_char(split_char);
}

string_splitter::~string_splitter()
{
}

void string_splitter::set_split_char(const char* split_char)
{
	if (split_char == NULL)
	{
		this->split_char = default_split_char;
	}
	else
	{
		this->split_char = split_char;
	}
}

size_t string_splitter::split_on_string_buffer(char* intput_string)
{
	char *token = NULL;

	char *pos = strtok_r(intput_string, split_char.c_str(), &token);

	if (splite_once)
	{
		if (pos != NULL)
		{
			vec_result.push_back(pos);
			vec_result.push_back(token);
		}
		else
		{
			vec_result.push_back(intput_string);
		}
	}
	else
	{
		while (pos != NULL)
		{
			vec_result.push_back(pos);
			pos = strtok_r(NULL, split_char.c_str(), &token);
		}
	}

	return size();
}

size_t string_splitter::split(const char* intput_string)
{
	vec_result.clear();

	if (intput_string == NULL || strlen(intput_string) == 0)
	{
		return 0;
	}

	size_t input_len = strlen(intput_string);

	if (input_len == 0)
	{
		return 0;
	}

	size_t buffer_len = input_len + 1;

	char* buffer = new char[buffer_len];
	memset(buffer, 0, buffer_len);
	strncpy(buffer, intput_string, input_len);

	split_on_string_buffer(buffer);

	delete[] buffer;

	return vec_result.size();
}

size_t string_splitter::size() const
{
	return vec_result.size();
}

bool string_splitter::empty() const
{
	return vec_result.empty();
}

void string_splitter::spliteonce(bool trun_on)
{
	splite_once = trun_on;
}

CONST_STRINGVECITR string_splitter::begin() const
{
	return vec_result.begin();
}

CONST_STRINGVECITR string_splitter::end() const
{
	return vec_result.end();
}

const std::string& string_splitter::operator[](size_t index)
{
	if (index >= vec_result.size())
	{
		static std::string empty_string;
		return empty_string;
	}

	return vec_result[index];
}

const STRINGVEC& string_splitter::data() const
{
	return vec_result;
}

void string_splitter::data_swap(STRINGVEC& vec)
{
	vec_result.swap(vec);
}


//////////////////////////////////////////////////////////////////////////
CURLArgsSplitter::CURLArgsSplitter()
{
	m_strFirstSpliter = "&";
	m_strSecondSpliter = "=";
}

CURLArgsSplitter::~CURLArgsSplitter()
{

}

void CURLArgsSplitter::set_split_char(string & str1Key, string& str2Key)
{
	m_strFirstSpliter = str1Key;
	m_strSecondSpliter = str2Key;
}

void CURLArgsSplitter::parser(string& strArgs)
{
	m_strArgs = strArgs;
	string_splitter cArgsSpliter(m_strFirstSpliter.c_str());
	cArgsSpliter.split(m_strArgs.c_str());
	for(int ii = 0; ii < cArgsSpliter.size(); ii ++)
	{
		string strOneArg = cArgsSpliter[ii];
		string_splitter cOneArgSplitter(m_strSecondSpliter.c_str());
		cOneArgSplitter.split(strOneArg.c_str());
		if(cOneArgSplitter.size()!= 2)
		{
			
		}
		else
		{
			m_ArgsMap[cOneArgSplitter[0]] = cOneArgSplitter[1];
		}
	}
}


std::string CURLArgsSplitter::getArgValByKey(string & strKey)
{
	stringMap::iterator itr = m_ArgsMap.find(strKey);
	if ( itr != m_ArgsMap.end() )
		return (*itr).second;
	else
		return string("");
}