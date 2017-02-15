#ifndef _MEMORY_HELPER_MACRO_H
#define _MEMORY_HELPER_MACRO_H
#include "autolock.h"

#define INIT_LOCK(name)			pthread_mutex_init(&name, NULL)
#define DEL_LOCK(name)			pthread_mutex_destroy(&name)
#define ENTER_LOCK(name)		auto_mutext_lock cAILock(&name)

#define INIT_LOCKEX(name, type)	\
	pthread_mutexattr_t attr;\
	pthread_mutexattr_init(&attr);\
	pthread_mutexattr_settype(&attr,type);\
	pthread_mutex_init(&name,&attr);\


#define SIMPLE_ZEROMEMORY(s)		\
	memset(&s, 0, sizeof(s))

#define SIMPLE_COPYOBJECT(obj1, obj2)	\
	memcpy_s(&obj1, sizeof(obj1), &obj2, sizeof(obj1))

#define DELETE_PTR(p)	\
	delete p;	\
	p = NULL;

#define DELETE_PTRA(p)	\
	delete[]p;	\
	p = NULL;

#define MALLOC_PTR(type, count)	(type*)malloc(sizeof(type) * count)

#define FREE_PTR(in)	\
	free(in);	\
	in = NULL;

#define  CHECK_VALID_PRT(ptr) \
if(NULL == ptr)\
{\
	return ;\
}


#define  CHECK_VALID_PRT_RETURN(ptr, retrunVal) \
if(NULL == ptr)\
{\
	return retrunVal;\
}

#endif // _MEMORY_HELPER_MACRO_H