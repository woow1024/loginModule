#pragma once

#define GW_DLLIMPORT							__declspec(dllimport)
#define GW_DLLEXPORT							__declspec(dllexport)

#define GW_C_DLLIMPORT							extern "C" GW_DLLIMPORT
#define GW_C_DLLEXPORT							extern "C" GW_DLLEXPORT 

#define GW_NAMECALL			

#define GW_CMDLINE_SIGN							_T("gwcmd?")

#define GW_HTTPHEAD_CONENT_TYPE					_T("GW-WEB-CONENT-TYPE")
#define GW_HTTPHEAD_CLIENT_VER					_T("GW-CLIENT-VER")
#define GW_HTTPHEAD_CLIENT_BUILD_NO				_T("GW-CLIENT-BUILD-NO")


#define FRAMEPLUGIN_BASE_ID						19811201
#define VIEWPLUGIN_BASE_ID						20091208

#define GWBOOL									BOOL 
#define GWINT									int
#define GWUINT									unsigned int
#define GWVOID									void
#define GWLONG									long
#define GWRESULT								LRESULT

#define COLOR_BLACK								RGB(0, 0, 0)			// ºÚÉ«
#define COLOR_WHITE								RGB(255, 255, 255)		// °×É«

#define GW_INVALID_UINT							(UINT)-1

#define GW_ARRAYSIZE(_Array)					(sizeof(_Array) / sizeof(_Array[0]))	
#define TYPE_CAST(type, obj)					(dynamic_cast<type>((obj)))
#define SAFE_GETVIEW(obj)						TYPE_CAST(CView*, obj)
#define SAFE_GETDLG(obj)						TYPE_CAST(CDialog*, obj)

#ifdef _X86_
#ifdef _DEBUG
#define GWDebugBreak()							 _asm { int 3 }
#else	
#define GWDebugBreak()
#endif
#else
#define GWDebugBreak()							DebugBreak()
#endif

#ifdef _DEBUG
#define WriteDebugMessage(content, ...)			GW_Log::WriteDebugMessageImp(__FILE__, __LINE__, LogNormal, content, __VA_ARGS__)
#define WriteLogMessage(level, content, ...)	GW_Log::WriteDebugMessageImp(__FILE__, __LINE__, level, content, __VA_ARGS__)
#else
#define WriteDebugMessage(content, ...)			GW_Log::WriteDebugMessageImp(LogNormal, content, __VA_ARGS__)
#define WriteLogMessage(level, content, ...)	GW_Log::WriteDebugMessageImp(level, content, __VA_ARGS__)
#endif

#define SafeStringCopy(dest, src)				ASSERT(sizeof(dest) != sizeof(char *)); StringCchCopy(dest, sizeof(dest), src)

#define PostDirectMessage(content)				GW_Msg::SendDirectMessage(content, this, NULL, TRUE)

#define GWGetInterface(theClass, IName)			(theClass == NULL ? NULL : (IName*)theClass->GetInterface(&__uuidof(IName)))

#define GWSTR2(x) #x
#define GWSTR(x)  GWSTR2(x)

#define GW_message(text) message(__FILE__ "(" GWSTR(__LINE__) "):" #text)
#define GW_VIEWMACRO(macro)	GWSTR2(macro)

#define DECLARE_MAP(key, value, name)	\
	typedef map<key, value>	name##MAP;	\
	typedef map<key, value>::iterator name##MAPITR;  \
	typedef map<key, value>::const_iterator CONST_##name##MAPITR;

#define DECLARE_VECTOR(value, name)	\
	typedef vector<value>  name##VEC;	\
	typedef vector<value>::iterator name##VECITR;	\
	typedef vector<value>::const_iterator CONST_##name##VECITR;

#define DECLARE_LIST(value, name)	\
	typedef list<value>  name##LST;	\
	typedef list<value>::iterator name##LSTITR;		\
	typedef list<value>::const_iterator CONST_##name##LSTITR;

#define VECTOR_MERGE(vectDest, vectorOrg, bClearFirt) \
	if(bClearFirt) \
{\
	vectDest.clear();\
}\
	if(FALSE == vectorOrg.empty())\
{\
	vectDest.insert(vectDest.end(), vectorOrg.begin(), vectorOrg.end());\
}

#define DELETE_PTR(p)	\
	delete p;	\
	p = NULL;
#define DELETE_PTRA(p)	\
	delete[]p;	\
	p = NULL;

#define SIMPLE_FIND_OBJECT(c, o) std::find(c.begin(), c.end(), o)