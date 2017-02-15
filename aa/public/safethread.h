#ifndef _THREADCENTER_H
#define _THREADCENTER_H

#include <easywork.h>
#include <std_macro.h>

#define INVALID_PTHREAD_T		0

template <typename _Ty, size_t N = 1> class safe_thread_base
{
protected:
	DECLARE_VECTOR(pthread_t, THREADIDVEC)

	typedef void (_Ty::*mem_work_func)(void*);
	typedef void* (*work_func)(void*);

	struct safe_thread_help_param	{
		_Ty*			this_obj;
		mem_work_func 	mem_func;
		void*			mem_param;
	} ;

public:
	safe_thread_base()
	{
		vec_threadid.resize(N, INVALID_PTHREAD_T);
		exit_signal_broadcast = false;
		pthread_cond_init(&cond_thread_exit, NULL);
		pthread_mutex_init(&mutex_thread_exit, NULL);
	}

	virtual ~safe_thread_base() 
	{
		broadcast_quit_message();
		join_all_thread();

		pthread_cond_destroy(&cond_thread_exit);
		pthread_mutex_destroy(&mutex_thread_exit);
	}

	void join_all_thread()
	{
		for (THREADIDVEC::iterator itr = vec_threadid.begin(); itr != vec_threadid.end(); ++itr)
		{
			pthread_t tid = *itr;
			
			if (tid == INVALID_PTHREAD_T)
			{
				continue;
			}

			pthread_join(tid, NULL);
			*itr = INVALID_PTHREAD_T;
		}
	}
	
	void safe_detach_thread(size_t thread_index)
	{
		if (thread_index >= vec_threadid.size())
		{
			return;
		}

		pthread_t tid = vec_threadid[thread_index];

		if (tid != INVALID_PTHREAD_T)
		{
			pthread_detach(tid);

			vec_threadid[thread_index] = INVALID_PTHREAD_T;
		}
	}

	void broadcast_quit_message()
	{
		if (exit_signal_broadcast)
		{
			return;			
		}

		pthread_cond_broadcast(&cond_thread_exit);
		exit_signal_broadcast = true;
	}
	
	bool safe_start_thread(mem_work_func func, void* param, size_t thread_index = 0)
	{
		if (thread_index >= vec_threadid.size())
		{
			return false;
		}

		safe_detach_thread(thread_index);
		
		vec_threadid[thread_index] = create_work_thread(func, param);
		
		return vec_threadid[thread_index] != INVALID_PTHREAD_T;
	}

	bool safe_start_thread(work_func func, void* param, size_t thread_index = 0)
	{
		if (thread_index >= vec_threadid.size())
		{
			return false;
		}

		safe_detach_thread(thread_index);

		vec_threadid[thread_index] = create_work_thread(func, param);

		return vec_threadid[thread_index] != INVALID_PTHREAD_T;
	}
	
	void join_thread(size_t thread_index)
	{
		if (thread_index >= vec_threadid.size())
		{
			return;
		}

		pthread_t tid = vec_threadid[thread_index];

		if (tid == INVALID_PTHREAD_T)
		{
			return;
		}

		int ret_value = pthread_join(tid, NULL);

		if (ret_value == 0)
		{
			vec_threadid[thread_index] = 0;
		}

		return;
	}
protected:
	pthread_t create_work_thread(mem_work_func func, void* param)		
	{
		safe_thread_help_param* help_param = new safe_thread_help_param;
		help_param->this_obj = static_cast<_Ty*>(this);
		help_param->mem_func = func;
		help_param->mem_param = param;
		
		pthread_t sid;

		int ret_value = pthread_create(&sid, NULL, thread_help_func, help_param);

		return (ret_value == 0) ? sid : INVALID_PTHREAD_T;	
	}

	pthread_t create_work_thread(work_func func, void* param)
	{
		pthread_t sid;

		int ret_value = pthread_create(&sid, NULL, func, param);

		return (ret_value == 0) ? sid : INVALID_PTHREAD_T;
	}

	static void* thread_help_func(void* param)
	{
		safe_thread_help_param *help_param = (safe_thread_help_param*)param;

		(help_param->this_obj->*help_param->mem_func)(help_param->mem_param);
		
		delete help_param;
		return NULL;
	}
protected:
	THREADIDVEC	vec_threadid;	
	bool exit_signal_broadcast;
	pthread_cond_t	cond_thread_exit;
	pthread_mutex_t mutex_thread_exit;
};


#endif
