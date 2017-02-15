#if !defined(StringUtil_H)
#define StringUtil_H


#include <algorithm>
#include <functional>
#include <stdarg.h>
#include <stdio.h> 
#include <string>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////
// StringUtil
//
// Purpose:		provides common utility methods for strings


class StringUtil
{
public:
	StringUtil ()
	{}


	static
	void trimRight ( string & str )
	{
		// trim spaces right
		trimRight( str, " " );

		/*
		// find last that is not a space
		string::reverse_iterator last = 
		std::find_if( str.rbegin(), str.rend(), std::bind2nd(std::not_equal_to<TCHAR>(),' ') );

		// remove spaces from end (here we use the knowledge that a reverse_terator 
		// is based on an iterator refer include file xstring
		str.erase( (string::iterator &) last, str.end() );
		*/
	}

	static
	void trimRight( string & str, char chTarget )
	{
		// setup buffer
		char buffer[2];
		buffer[0] = chTarget;
		buffer[1] = '\0';

		trimRight( str, buffer );

		/*
		// find last that is not a space
		string::reverse_iterator last = 
		std::find_if( str.rbegin(), str.rend(), 
		              std::bind2nd(std::not_equal_to<TCHAR>(),chTarget) );

		// remove spaces from end (here we use the knowledge that a reverse_terator 
		// is based on an iterator refer include file xstring
		str.erase( (string::iterator &) last, str.end() );
		*/
	}

	static
	void trimRight ( string & str, const char* lpszTargets )
	{
		// find first that is not space
		long pos  = str.find_last_not_of(lpszTargets);

		// if start then nothing found
		if ( pos == str.size() || pos == -1 )
			return;

		// remove from string
		str.erase(0,pos) ;
		/*
		// find last that is not a space
		string::reverse_iterator last = 
		std::find_if( str.rbegin(), str.rend(), 
		              std::bind2nd(std::not_equal_to<TCHAR>(),lpszTargets) );

		// remove spaces from end (here we use the knowledge that a reverse_terator 
		// is based on an iterator refer include file xstring
		str.erase( (string::iterator &) last, str.end() );
		*/
	}

	static
	void trimLeft ( string & str, char chTarget )
	{
		// setup buffer
		char buffer[2];
		buffer[0] = chTarget;
		buffer[1] = '\0';

		trimLeft( str, buffer );
	}

	static
	void trimLeft ( string & str, const char* lpszTargets )
	{
		// find first that is not space
		long pos  = str.find_first_not_of(lpszTargets);

		// if start then nothing found
		if ( pos == 0  || pos == -1 )
			return;

		// remove from string
		str.erase(0,pos) ;
	}


	static
	void trimLeft ( string & str )
	{
		// trim spaces
		trimLeft( str, " " );
	}



	static
	int replace ( string & str, char chOld, char chNew )
	{
		// replace if found
		str.replace( str.begin(), str.end(), chOld, chNew );

		return 0;
	}

	static
	int replace ( string & str, const char* lpszOld, const char* lpszNew )
	{
		// replace if found
		str.replace( str.begin(), str.end(), lpszOld, lpszNew );

		return 0;
	}


	static
	string left ( const string & str, long pos )
	{
		return str.substr( 0, pos );
	}

	static
	string mid ( const string & str, long pos, long len = -1 )
	{
		if ( len == -1 )
			len = str.size() - 1;

		return str.substr( pos, len );
	}

	static
	void makeUpper ( string & str )
	{
		// reverse case of string
		for( string::iterator p = str.begin(); p != str.end(); p++ )
		{
			if( islower( *p ) )
				*p = _toupper( *p );
		}

	}


	static
	void makeLower ( string & str )
	{
		// reverse case of string
		for( string::iterator p = str.begin(); p != str.end(); p++ )
		{
		  if( isupper( *p ) )
			 *p = _tolower( *p );
		}

	}

	static
	bool compareNoCase ( const string & strIn1, const char* strIn2 )
	{
		// come back later and do compare with 
		// conversion inline
		string str1 = strIn1;
		makeLower( str1 );

		string str2 = strIn2;
		makeLower( str2 );

		if ( str1 == str2 )
			return true;
		else
			return false;
	}


	static
	bool compareNoCase ( const string & strIn1, const string & strIn2 )
	{
		// come back later and do compare with 
		// conversion inline
		string str1 = strIn1;
		makeLower( str1 );

		string str2 = strIn2;
		makeLower( str2 );

		if ( str1 == str2 )
			return true;
		else
			return false;
	}

	static
	bool loadString ( string & str, const char* loadStr )
	{
		if ( loadStr )
		{
			str = loadStr;
			return true;
		}
		else
			return false;
	}


	static
	void formatString ( string & str, char* lpszFormat, ... )
	{
		va_list args;
		va_start(args, lpszFormat);

		// buffer for format
		int   len;
		char buffer[512] = ("\0");

		// perform format
#ifndef _WIN32
		len = vsnprintf(buffer, 512, lpszFormat, args);
#else

		len = _vsntprintf(buffer, 512, lpszFormat, args);
#endif

		// if there was no error
		if ( len >= 0 )
			str = buffer;

		va_end(args);
	}

	static
	char* getBufferString ( string & str, long length )
	{
		
		//string::pointer ptr = str.get_allocator().allocate( length, NULL );
		//str.assign( ptr );

		
		// get allocator for string
		//string::allocator_type allocator = 
		//str.get_allocator();

		// setup buffer for copy
		str.resize( length );

		// copy data
		char* ptr = (char*) str.c_str();
		


		return ptr;
	}

	static
	int intVal ( string strVal )
	{
		int nVal = 0;
		trimLeft(strVal);
		for( int index = 0; index < strVal.size(); ++index )
			nVal = nVal*10 + strVal[index] - '0';

		return nVal;
	}

	static
	int monthFromStr ( const string& str )
	{
		char  aMonths[][16] = 
		{
			"xxx", "jan", "feb", "mar", "apr", "may", "jun",
			"jul", "aug", "sep", "oct", "nov", "dec" 
		};
		int nMonth=1;
		for( ; nMonth <= 12; ++nMonth )
		{
			if ( compareNoCase( str, aMonths[nMonth] ) )
				break;
		}

		return nMonth;
	}


};

#endif