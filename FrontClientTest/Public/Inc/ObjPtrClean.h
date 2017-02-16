#pragma  once

struct ObjPtrClean			// 指针对象清理
{
	template <typename T> void operator()(T* ptr)
	{
		delete ptr;
	}

	template <typename _Key, typename _Value> void operator()(pair<_Key, _Value> _P)
	{
		delete _P.second;
	}
};