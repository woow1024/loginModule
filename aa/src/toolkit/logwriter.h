#ifndef _LOGWRITER_H
#define _LOGWRITER_H

#define ENABLE_MULTITHREAD_MODE		0
#define USER_THREAD_WRITE	  		1

#if ENABLE_MULTITHREAD_MODE
#include <pthread.h>
#endif

#include <stdarg.h>
#include <string>
#include <string.h>
#include <errno.h>
#include <vector>
#include "safequeue.h"
#include "safethread.h"
#include "safequeuethread.h"

#define EXPORT_LOG_SUPPORT()		extern log_writer local_log;

#define INIT_LOG(filename, add_date, add_time, split_by_date, max_filesize)\
	log_writer local_log(filename, add_date, add_time, split_by_date, max_filesize);

#define ERROR_EXIT(text, ...)\
	local_log.format_and_write (text, ##__VA_ARGS__);\
	exit(1)
#define WRITE_LOG(text) local_log.write (text, strlen(text))
#define WRITE_FORMAT_LOG(text, ...)		local_log.format_and_write (text, ##__VA_ARGS__)
#define WRITE_ERROR_LOG(error, text, ...)	local_log.write_error (error, text, ##__VA_ARGS__)
#define WRITE_LOG_WITH_ERROR(text, ...)	WRITE_ERROR_LOG(errno, text, ##__VA_ARGS__)

#define INVALID_FILENO		(-1)

#define CLOSE_FILENO(fileno)\
	if (fileno != INVALID_FILENO) {\
		::close(fileno);\
		fileno = INVALID_FILENO; }

struct tagOneLog
{
	char* text;
	size_t text_len;
	bool write_error;
	int error_no;
	tagOneLog()
	{
		memset(this, 0, sizeof(tagOneLog));
	}

	~tagOneLog()
	{
		DELETE_PTRA(text);
	}
};

class log_writer : public CSafeQueueThread<tagOneLog*>
{
	typedef std::vector<struct iovec>	vec_iovec;
public:
	log_writer();
	log_writer(const char* filename, bool add_date = true, bool add_time = true, bool split_by_date = false, size_t max_filesize = 0);
	~log_writer(void);

	void init_writer(const char* filename, bool add_date, bool add_time, bool split_by_date, size_t max_filesize);
	void close(); 
	const std::string get_error_message_text();
	const std::string get_error_message_text(int error_no);

	size_t write_error(int error_no, const char* text, ...);
	size_t write(const char* text, size_t text_len = 0);
	size_t write(int error_no, const char* text, size_t text_len = 0);
	size_t format_and_write(const char* text, ...);
	
private:
	size_t safe_writev_to_file(int fileno, struct iovec* content, int count); 
	int	get_current_date();
	bool check_file_state();		

	size_t vformat_and_write(bool write_error, int error_no, const char* text, va_list ap);
	size_t write_impl(const char* text, size_t text_len, bool write_error, int error_no);
	size_t writeintofile(const char* text, size_t text_len, bool write_error, int error_no);
	void make_base_iovec(vec_iovec& iovec, const char* text, size_t text_len);
	void add_iobuffer_to_iovec(vec_iovec& vec, void* buffer, size_t buffer_len);
	bool create_new_log_file();		
private:
	virtual void ClearAllData();
	virtual bool HandOnePacket(tagOneLog* pOnePacket);
private:
	int				fileno;			
	int				last_date;	
	int				next_filename_no;
	size_t			max_filesize;	
	bool 			split_by_date;			
	bool			add_time;				
	bool			add_date;		
	std::string		log_file_name;
	std::string 	last_log_file_name;

#if ENABLE_MULTITHREAD_MODE
	pthread_mutex_t		file_mutex_lock;
#endif	
};

extern log_writer local_log;

#endif
