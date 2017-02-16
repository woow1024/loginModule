#ifndef _JTL_SMART_OBJECT_H
#define _JTL_SMART_OBJECT_H

typedef void (__cdecl *CDECL_CLEARUP_FUNC)(DWORD inner_handele);
typedef void (__stdcall *STDCALL_CLEARUP_FUNC)(DWORD inner_handele);

template <class _handle_type, class _func_type, _func_type clearup_func, DWORD _invalid_handle_value = NULL> 
class smart_handle
{
public:
	smart_handle(_handle_type handle = _invalid_handle_value) 
	{
		inner_handle = (DWORD)handle;
	}
	~smart_handle()
	{
		close();
	}

	void close()
	{
		if (inner_handle != _invalid_handle_value)
		{
			clearup_func(inner_handle);
			inner_handle = _invalid_handle_value;
		}
	}

	void attach(_handle_type handle)
	{
		close();
		inner_handle = (DWORD)handle;
	}

	_handle_type operator=(_handle_type handle)
	{
		attach(handle);
		return (_handle_type)inner_handle;
	}

	operator _handle_type()
	{
		return (_handle_type)inner_handle;
	}

	BOOL isvalid()
	{
		return inner_handle != _invalid_handle_value;
	}
private:
	DWORD	inner_handle;
};

#define DECLARE_SMART_OBJ_CLASS(CLASSNAME, HANDLETYPE, CLEARUPTYPE, CLEARUPFUNC)	typedef smart_handle<HANDLETYPE, CLEARUPTYPE, (CLEARUPTYPE)CLEARUPFUNC> CLASSNAME;
#define DECLARE_SMART_OBJ_CLASS_EX(CLASSNAME, HANDLETYPE, CLEARUPTYPE, CLEARUPFUNC, INVALIDVALUE)	typedef smart_handle<HANDLETYPE, CLEARUPTYPE, (CLEARUPTYPE)CLEARUPFUNC, (DWORD)INVALIDVALUE> CLASSNAME;

DECLARE_SMART_OBJ_CLASS(smart_file_p, FILE*, CDECL_CLEARUP_FUNC, fclose)									// fopen
DECLARE_SMART_OBJ_CLASS_EX(smart_file_h, HANDLE, STDCALL_CLEARUP_FUNC, CloseHandle, INVALID_HANDLE_VALUE)	// CreateFile
DECLARE_SMART_OBJ_CLASS(smart_filemap_h, HANDLE, STDCALL_CLEARUP_FUNC, CloseHandle)							// CreateFileMapping
DECLARE_SMART_OBJ_CLASS(smart_mapview_p, void*, STDCALL_CLEARUP_FUNC, UnmapViewOfFile)						// MapViewOfFile		
DECLARE_SMART_OBJ_CLASS(smart_library_m, HMODULE, STDCALL_CLEARUP_FUNC, FreeLibrary)						// LoadLibrary 		

class smart_buffer				// auto malloc and free
{
public:
	smart_buffer(int len = 0)
	{
		inner_buff	   = NULL;
		inner_len	   = 0;
		inner_capacity = 0;

		if (len > 0)
		{
			alloc(len);
		}
	}

	~smart_buffer()
	{
		release();
	}

	void release()
	{
		free(inner_buff);

		inner_buff     = NULL;
		inner_len      = 0;
		inner_capacity = 0;
	}

	BOOL alloc(size_t len)
	{
		if (len <= inner_capacity)
		{
			inner_len = len;
			memset(inner_buff, 0, inner_len);
			return TRUE;
		}

		release();

		inner_buff = (char*)malloc(len);

		if (inner_buff == NULL)
		{
			return FALSE;
		}

		inner_len		 = len;
		inner_capacity	 = len;

		memset(inner_buff, 0, inner_len);
		return TRUE;
	}

	operator char*()
	{
		return inner_buff;
	}

	char* getbuffer()
	{
		return inner_buff;
	}

	size_t getlength()
	{
		return inner_len;
	}

	bool isvliadpointer(void* address)
	{
		return address > inner_buff && address < (inner_buff + inner_len);
	}
private:
	char* inner_buff;
	size_t	inner_len;
	size_t  inner_capacity;
};






#endif
