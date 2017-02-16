#pragma once

#include <std_macro.h>
#include <map>
#include <string>
using namespace std;

DECLARE_VECTOR(std::string, STRINGVEC)

class string_splitter
{
public:
	string_splitter(const char* split_char = NULL);
	~string_splitter();
public:
	void set_split_char(const char* split_char);
	size_t split(const char* intput_string);
	size_t split_on_string_buffer(char* intput_string);

	size_t size() const;
	bool empty() const;

	void spliteonce(bool trun_on);

	CONST_STRINGVECITR begin() const;
	CONST_STRINGVECITR end() const;
	
	const string& operator[](size_t index);
	
	const STRINGVEC& data() const;
	void data_swap(STRINGVEC& vec);
private:
	string		split_char;				// 分隔符
	STRINGVEC		vec_result;				// 结果集
	bool			splite_once;
};

typedef map<string,string> stringMap;
class CURLArgsSplitter
{
public:
	CURLArgsSplitter();
	~CURLArgsSplitter();
public:
	std::string getArgValByKey(string & strKey); 
	void set_split_char(string & str1Key, string& str2Key);
	void parser(string& strArgs);
private:
	string m_strFirstSpliter;
	string m_strSecondSpliter;

	string m_strArgs;
	stringMap m_ArgsMap;
};