

#include <easywork.h>
#include <autolock.h>
#include "logwriter.h"

#define S_LOGFILEMODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH		
#define O_LOGFILEFLAG O_CREAT | O_WRONLY | O_APPEND
#define  MAX_LOG_FILE_LEN		(1024 * 1024 * 10)		//10M
size_t log_writer::safe_writev_to_file(int fileno, struct iovec* content, int count)
{
#if ENABLE_MULTITHREAD_MODE
	auto_mutext_lock lock(&file_mutex_lock);
#endif	
	size_t write_len = writev(fileno, content, count);	

	return write_len; 
}


log_writer::log_writer()
{
	char szFileName[128] = "DCLog";
#if 0
	time_t t = time(NULL);
	struct tm tm_now;
	localtime_r(&t, &tm_now);
	sprintf(szFileName, "log%04d%02d%02d_%02d%02d%02d.txt", (tm_now.tm_year + 1900), (tm_now.tm_mon + 1), tm_now.tm_mday,
				tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);
#endif
	init_writer(szFileName, true, true, true, MAX_LOG_FILE_LEN);
}

log_writer::~log_writer()
{
	close();

#if ENABLE_MULTITHREAD_MODE
	pthread_mutex_destroy(&file_mutex_lock);
#endif
}

log_writer::log_writer(const char* filename, bool add_date, bool add_time, bool split_by_date, size_t max_filesize)  
{
	init_writer(filename, add_date, add_time, split_by_date, max_filesize);
}
	

void log_writer::init_writer(const char* filename, bool add_date, bool add_time, bool split_by_date, size_t max_filesize)
{
	assert(filename != NULL);

	log_file_name = filename;
	
	this->split_by_date = split_by_date;
	this->max_filesize	= max_filesize;
	this->add_date		= add_date;
	this->add_time		= add_time;
	this->fileno		= INVALID_FILENO;
	this->last_date 	= get_current_date();

	next_filename_no	= 0;
#if ENABLE_MULTITHREAD_MODE
	pthread_mutex_init(&file_mutex_lock, NULL);
#endif

#if USER_THREAD_WRITE
	this->Start();
#endif
}

void log_writer::close()
{
	CLOSE_FILENO(fileno);
}

int log_writer::get_current_date()
{
	time_t t = time(NULL);
	struct tm tm_now;
	localtime_r(&t, &tm_now);

	return (tm_now.tm_year + 1900) * 10000 + (tm_now.tm_mon + 1) * 100 + tm_now.tm_mday;
}

bool log_writer::check_file_state()
{
	if (fileno == INVALID_FILENO)	
	{
		return false;
	}

	if (access(last_log_file_name.c_str(), F_OK) != 0)
	{
		return false;	
	}
	
	if (split_by_date && get_current_date() != last_date)	
	{
		next_filename_no = 0;
		last_date = get_current_date();
		return false;
	}

	if (max_filesize > 0)		
	{
		size_t file_len = (size_t)lseek(fileno, 0, SEEK_CUR);
		
		if (file_len >= max_filesize)	
		{
			++next_filename_no;
			return false;
		}
	}

	return true;
}

void log_writer::add_iobuffer_to_iovec(vec_iovec& vec, void* buffer, size_t buffer_len)
{
	struct iovec content;
	content.iov_base = buffer;
	content.iov_len = buffer_len;

	vec.push_back(content);
}

size_t log_writer::vformat_and_write(bool write_error, int error_no, const char* text, va_list ap)
{
	static const size_t buffer_len = 1024;
	char text_buffer[buffer_len];
	char* buffer_ptr = text_buffer;
	if(strlen(text)> 1000)
	{
		return 0;
	}

	va_list ap_bak;
	va_copy(ap_bak, ap);
	
	size_t write_len = vsnprintf(buffer_ptr, buffer_len, text, ap);
			
	if (write_len > buffer_len)
	{
		va_copy(ap, ap_bak);
		buffer_ptr = (char*)malloc(write_len + 1);
		write_len = vsnprintf(buffer_ptr, write_len, text, ap);
	}
			

	write_len = write_impl(buffer_ptr, write_len, write_error, error_no);
	
	if (buffer_ptr != text_buffer)
	{
		free(buffer_ptr);
	}
			
	return write_len;
}

size_t log_writer::write(const char* text, size_t text_len)
{ 
	return write_impl(text, text_len, false, 0);
}

size_t log_writer::format_and_write(const char* text, ...)
{	 
	va_list ap;
	va_start(ap, text);
	
	size_t write_len = vformat_and_write(false, 0, text, ap);
				
	va_end(ap);
		
	return write_len;
}


size_t log_writer::write_error(int error_no, const char* text, ...)
{ 
	va_list ap;
	va_start(ap, text);

	size_t write_len = vformat_and_write(true, error_no, text, ap);

	va_end(ap);

	return write_len;
}

const std::string log_writer::get_error_message_text()
{
	return get_error_message_text(errno);
}

const std::string log_writer::get_error_message_text(int error_no)
{
	char error_buffer[1024] = { 0 };
	char * error_ptr = strerror_r(error_no, error_buffer, sizeof(error_buffer));
	return error_ptr;
	//return error_buffer;
}

size_t log_writer::write_impl(const char* text, size_t text_len, bool write_error, int error_no)
{
	if(NULL == text || text_len <= 0)
	{
		return 0;
	}
#if USER_THREAD_WRITE
	tagOneLog * pOneLog = new tagOneLog;
	pOneLog->text_len = text_len;
	pOneLog->write_error = write_error;
	pOneLog->error_no = error_no;
	pOneLog->text = new char [text_len + 1];
	memcpy(pOneLog->text, text, text_len);
	pOneLog->text[text_len] = '\0';
	m_cDataQueque.push(pOneLog);
#else
	writeintofile(text, text_len, write_error, error_no);
#endif
	return 1;
}

size_t log_writer::writeintofile(const char* text, size_t text_len, bool write_error, int error_no)
{
	if (!check_file_state())
	{
		if (!create_new_log_file())
		{
			return 0;
		}
	}
	
	assert (fileno != INVALID_FILENO);

	if (fileno == INVALID_FILENO)
	{
		return 0;
	}
	
	static char LINE_CTRF[] = "\n";
	static char DATE_TIME_SPLITE[] = ": ";
	static char ERROR_SPLITE[] = " - ";
	vec_iovec iovec;
	
	static int nStaticLogNum = 0;
	nStaticLogNum ++;
	int nRemain = this->GetSize();
	char szTemp[64];
	size_t nLogLen = snprintf(
		szTemp, 
		sizeof(szTemp), 
		"[%d  %d] ",
		nStaticLogNum,
		nRemain);
	add_iobuffer_to_iovec(iovec, szTemp, nLogLen);

	if (add_date || add_time)
	{
		char time_buffer[64];
		size_t time_buffer_len = 0;
		time_t t = time(NULL);
		struct tm tm_now;
		localtime_r(&t, &tm_now);
	
		if (add_date)
		{
			time_buffer_len = snprintf(
									time_buffer, 
									sizeof(time_buffer), 
									"%04d-%02d-%02d ",
									tm_now.tm_year + 1900,
									tm_now.tm_mon + 1,
									tm_now.tm_mday
									);			
		}
	
		if (add_time)
		{
			time_buffer_len += snprintf(
										time_buffer + time_buffer_len, 
										sizeof(time_buffer) - time_buffer_len, 
										"%02d:%02d:%02d ",
										tm_now.tm_hour, 
										tm_now.tm_min,
										tm_now.tm_sec
										);	
		}

		add_iobuffer_to_iovec(iovec, time_buffer, time_buffer_len);
		add_iobuffer_to_iovec(iovec, DATE_TIME_SPLITE, strlen(DATE_TIME_SPLITE));
	}
	
	add_iobuffer_to_iovec(iovec, (void*)text, text_len == 0 ? strlen(text) : text_len);

	if (write_error)
	{
		std::string error_msg = get_error_message_text(error_no);
		add_iobuffer_to_iovec(iovec, &ERROR_SPLITE, strlen(ERROR_SPLITE));
		add_iobuffer_to_iovec(iovec, (void*)error_msg.c_str(), error_msg.length());
	}
	
	add_iobuffer_to_iovec(iovec, &LINE_CTRF, strlen(LINE_CTRF));
	
	size_t write_len = safe_writev_to_file(fileno, &iovec[0], iovec.size());
	
#ifdef DEBUG_MODE
	for (vec_iovec::iterator itr = iovec.begin(); itr != iovec.end(); ++itr) 
	{
		cout << (char*)itr->iov_base;
	}
	
	cout << flush;
#endif
	
	return write_len;
}

bool log_writer::create_new_log_file()
{
	close();

	if (!split_by_date&& max_filesize == 0)	// 单日志模式
	{
		last_log_file_name = log_file_name;
	} 
	else
	{
		char filename_buffer[512];
		
		if (split_by_date)			// 按日期分割
		{
			if (max_filesize > 0)
			{
				do					
				{
					sprintf(filename_buffer, "%s_%08d_%d",
						log_file_name.c_str(),
						last_date,
						next_filename_no);

					if (access(filename_buffer, F_OK) != 0)
					{
						break;
					}

					++next_filename_no;
				} while(true);
			}
			else
			{
				sprintf(filename_buffer, "%s_%08d", 
					log_file_name.c_str(),
					last_date);
			}
		}
		else
		{
			do  
			{
				sprintf(filename_buffer, "%s_%d",
						log_file_name.c_str(),
						next_filename_no);

				if (access(filename_buffer, F_OK) != 0)
				{
					break;
				}

				++next_filename_no;
			} while (true);
		}

		last_log_file_name = filename_buffer;
	}

	fileno = open(last_log_file_name.c_str(), O_LOGFILEFLAG, S_LOGFILEMODE);

	if (fileno == INVALID_FILENO)
	{
		cout << get_error_message_text() << endl;
		assert (fileno != INVALID_FILENO);
	}
	
	return true;
}



void log_writer::ClearAllData()
{
	while(1)
	{
		tagOneLog* pOnePacket = m_cDataQueque.pop();
		if(NULL == pOnePacket)
		{	
			return;
		}
		DELETE_PTR(pOnePacket);
	}
}

bool log_writer::HandOnePacket(tagOneLog* pOnePacket)
{
	if(NULL == pOnePacket)
	{
		return false;
	}
	writeintofile(pOnePacket->text, pOnePacket->text_len, pOnePacket->write_error, pOnePacket->error_no);
	DELETE_PTR(pOnePacket);
	return true;
}