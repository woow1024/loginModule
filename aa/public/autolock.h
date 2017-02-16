#ifndef _AUTOLOCK
#define _AUTOLOCK

#include <pthread.h>

class auto_mutext_lock
{

public:
	auto_mutext_lock(pthread_mutex_t* mutex = NULL) : mutex_obj(mutex) {lock();}
	~auto_mutext_lock() { unlock(); }

	void lock()
	{
		if (mutex_obj)
		{
			pthread_mutex_lock(mutex_obj);
		}
	}

	void unlock()
	{
		if (mutex_obj)
		{
			pthread_mutex_unlock(mutex_obj);

			mutex_obj = NULL;
		}
	}
private:
	pthread_mutex_t* mutex_obj;
};




 




#endif 
