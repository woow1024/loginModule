#include "StdAfx.h"

#define _ENABLE_LUA_SUPPORT
#ifdef _ENABLE_LUA_SUPPORT

#include "LuaWrap.h"
#include <cassert>

#include "FuncLib.h"

extern "C"{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

#pragma comment(lib, "lua51.lib")

const char DEFAULT_CHUNK_NAME[] = "luawrap_chunk";

#define CHECKE_LUA_STATE()	\
	assert(IsLuaStateValid());\
	if (!IsLuaStateValid())	\
	{	\
		m_strError = "lua state is invalid";	\
		return false;	\
	}	\
	else	\
	{	\
		m_strError = "";	\
	}

#define CHECK_LUA_ERROR(err)	\
	if (err != 0)	\
	{	\
		m_strError = lua_tostring(m_pL, -1);	\
		lua_pop(m_pL, 1);	\
		WriteLogMessage(LogWarning, m_strError);	\
		return false;		\
	}

#define EXCUTE_CHUNK(b)	\
	if (b) \
	{	\
		Excute();	\
	}

LuaWrap::LuaWrap(void) : m_pL(NULL)
{
	InitLuaState();
}

void LuaWrap::CleanLuaState()
{
	if (m_pL)
	{
		__try
		{
			lua_close(m_pL);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
#ifdef _DEBUG
			ASSERT(FALSE);
			WriteLogMessage(LogError, _T("LuaWrap::CleanLuaState函数发生异常"));
#endif
		}
	}
}

LuaWrap::~LuaWrap(void)
{
	CleanLuaState();
}

const LuaString LuaWrap::GetStackDump() 
{
	LuaString strStack;
	char szItemBuffer[1024];

	int top = lua_gettop(m_pL);

	for (int i = 1; i <= top; i++)
	{ 
		int t = lua_type(m_pL, i);

		switch (t) 
		{
		case LUA_TSTRING:  /* strings */
			{
				sprintf_s(szItemBuffer, "type=string value='%s'", lua_tostring(m_pL, i));
			}
			break;
	   case LUA_TBOOLEAN:  /* booleans */
		   {
			   sprintf_s(szItemBuffer, "type=booleans value='%s'", lua_toboolean(m_pL, i) ? "true" : "false");
		   }
		   break;
	   case LUA_TNUMBER:  /* numbers */
		   {
			   sprintf_s(szItemBuffer, "type=numbers value=%g", lua_tonumber(m_pL, i));
		   }
		   break;
	   default:  /* other values */
		   {
			   sprintf_s(szItemBuffer, "type=%s", lua_typename(m_pL, t));
		   }
		   break;
		}

		strStack += szItemBuffer;
		strStack += "\n";
	}

	return strStack;
}


bool LuaWrap::Test()
{
	CHECKE_LUA_STATE()

	char buff[256];

	int error;

	lua_State *L = m_pL;  /* opens Lua */

	while (fgets(buff, sizeof(buff), stdin) != NULL) {

		error = luaL_loadbuffer(L, buff, strlen(buff),

			"line");

		if (error) {

			fprintf(stderr, "%s", lua_tostring(L, -1));

			lua_pop(L, 1);/* pop error message from the stack */

		}
		else
		{
			lua_pcall(L, 0, 0, 0);
		}

	}

	return true;
}

bool LuaWrap::InitLuaState()
{
	assert(m_pL == NULL);
	m_pL = lua_open();

	if (m_pL)
	{
		luaL_openlibs(m_pL);
	}
	
	return IsLuaStateValid();
}

bool LuaWrap::LoadString(const char* chunk, bool bExcute)
{
	return LoadBuffer(chunk, strlen(chunk), bExcute);
}

bool LuaWrap::LoadFile(const char* filename, bool bExcute)
{
	CHECKE_LUA_STATE()

	int	error = luaL_loadfile(m_pL, filename);

	CHECK_LUA_ERROR(error)
	EXCUTE_CHUNK(bExcute)

	return true;
}

bool LuaWrap::LoadBuffer(const char* chunk, int nBufferLen, bool bExcute)
{
	CHECKE_LUA_STATE()

	int	error = luaL_loadbuffer(m_pL, chunk, nBufferLen, DEFAULT_CHUNK_NAME);

	CHECK_LUA_ERROR(error)
	EXCUTE_CHUNK(bExcute)

	return true;
}

bool LuaWrap::Excute(const char* func, int nresults)
{
	CHECKE_LUA_STATE()

	if (!PushFunction(func))
	{
		return false;
	}
	
	return ExcuteLuaFunction(0, nresults);
}

bool LuaWrap::ExcuteFunc(const char* func, int nresults, const char* fmt, ...)
{
	CHECKE_LUA_STATE()

	if (fmt == NULL)
	{
		return Excute(func, nresults);
	}

	if (!PushFunction(func))
	{
		return false;
	}
	
	int nargs = strlen(fmt);

	__try
	{
		// 参数准备
		va_list arg_ptr;
		va_start(arg_ptr, fmt);

		const char* p = fmt;

		while (*p)
		{	
			switch(*p)
			{
			case 'd':	// 整数
				{
					lua_pushnumber(m_pL, va_arg(arg_ptr, int));
				}
				break;
			case 'f':	// double
				{
					lua_pushnumber(m_pL, va_arg(arg_ptr, double));
				}
				break;
			case 'b':	// bool
				{
					lua_pushboolean(m_pL, va_arg(arg_ptr, int));
				}
				break;
			case 's':	// string
				{
					lua_pushstring(m_pL, va_arg(arg_ptr, char*));
				}
				break;
			default:
				{
					m_strError = "wrong arg fmt string :";
					m_strError += fmt;
					return false;
				}
			}

			++p;
		}

		va_end(arg_ptr);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		m_strError = "resolv param error with fmt string :";
		m_strError += fmt;
		return false;
	}

	return ExcuteLuaFunction(nargs, nresults);
}

bool LuaWrap::PushFunction(const char* func)
{
	if (func && strcmp(func, "") != 0)
	{
		lua_getglobal(m_pL, func);	
	
		if (!lua_isfunction(m_pL, -1))
		{
			m_strError = "can't find function ";
			m_strError += func;
			WriteLogMessage(LogWarning, m_strError);
			return false;
		}
	}

	return true;
}

bool LuaWrap::ExcuteLuaFunction(int nargs, int nresults)
{
	if (nresults < 0)
	{
		nresults = 0;		
	}

	int	error = lua_pcall(m_pL, nargs, nresults, 0);

	CHECK_LUA_ERROR(error)

	return true;
}

int LuaWrap::ValueAsInt(int nDefault, int index, bool bPopTop)
{
	if (!lua_isnumber(m_pL, index))
	{
		return nDefault;
	}

	int nValue = lua_tointeger(m_pL, index);

	if (bPopTop)
	{
		lua_pop(m_pL, 1);
	}

	return nValue;
}

double LuaWrap::ValueAsDouble(double dbDefault, int index, bool bPopTop)
{
	if (!lua_isnumber(m_pL, index))
	{
		return dbDefault;
	}

	double dbValue = lua_tonumber(m_pL, index);

	if (bPopTop)
	{
		lua_pop(m_pL, 1);
	}

	return dbValue;
}

const LuaString LuaWrap::ValueAsString(int index, bool bPopTop)
{
	if (!lua_isstring(m_pL, index))
	{
		return NULL;
	}

	LuaString strValue = lua_tostring(m_pL, index);

	if (bPopTop)
	{
		lua_pop(m_pL, 1);
	}

	return strValue;
}

#endif