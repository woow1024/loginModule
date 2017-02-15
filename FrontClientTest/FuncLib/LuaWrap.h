#pragma once

#define _ENABLE_LUA_SUPPORT
#ifdef _ENABLE_LUA_SUPPORT

typedef struct lua_State lua_State;

//#define USE_STD_STRING

#ifdef USE_STD_STRING
#define LuaString std::string
#else
#define LuaString CString
#endif

#undef GW_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define GW_EXT_CLASS _declspec(dllexport) 
#else
#define GW_EXT_CLASS _declspec(dllimport)
#endif

class GW_EXT_CLASS LuaWrap
{
public:
	LuaWrap(void);
	~LuaWrap(void);

	bool IsLuaStateValid () const { return m_pL != NULL; }

	const LuaString& GetLastErrorStr() const { return m_strError; }

	bool LoadString(const char* chunk, bool bExcute = true);
	bool LoadBuffer(const char* chunk, int nBufferLen, bool bExcute = true);
	bool LoadFile(const char* filename, bool bExcute = true); 

	bool Excute(const char* func = NULL, int nresults = 0);
	bool ExcuteFunc(const char* func, int nresults, const char* fmt, ...);

	int ValueAsInt(int nDefault = 0, int index = -1, bool bPopTop = true);
	double ValueAsDouble(double dbDefault = 0.0, int index = -1, bool bPopTop = true);
	const LuaString ValueAsString(int index = -1, bool bPopTop = true);
	
	const LuaString GetStackDump();

	bool Test();
private:
	bool InitLuaState();
	void CleanLuaState();

	bool PushFunction(const char* func);
	bool ExcuteLuaFunction(int nargs = 0, int nresults = 0);
private:
	lua_State		*m_pL;
	LuaString		m_strError;
};
#endif