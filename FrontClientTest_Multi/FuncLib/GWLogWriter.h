#pragma once

#include <toolclass.h>

#include <gwtypes.h>
#include <io.h>

class logwriter
{
	class date_holder
	{
	public:
		int year;
		int month;
		int day;

		date_holder()
		{
			update_date();
		}

		bool update_date()
		{
			time_t new_time = time(NULL);
			struct tm t;
			localtime_s(&t, &new_time);

			t.tm_year += 1900; 
			t.tm_mon += 1;

			if (t.tm_mday == day
				&& t.tm_mon == month
				&& t.tm_year == year)
			{
				return false; 
			}

			day = t.tm_mday;
			month = t.tm_mon;
			year = t.tm_year;

			return true;
		}
	};
public:
	logwriter(const char* file_name, bool write_time = true, int max_file_length = 0, bool split_bydate = false)
	{
		init(file_name, write_time, max_file_length, split_bydate);
	}

	logwriter()
	{
		init(NULL);
	}

	~logwriter()
	{
		close();

		DeleteCriticalSection(&multi_thread_lock);
	}

	void init(const char* file_name = NULL, bool write_time = true, int max_file_length = 0, bool split_bydate = false)
	{
		if (file_name != NULL)
		{
			strcpy_s(log_file_name, file_name);
		}
		else
		{
			strcpy_s(log_file_name, "");
		}

		this->split_bydate		= split_bydate;
		this->max_file_length	= max_file_length;
		this->write_time		= write_time;
	
		//file_handle				= NULL;
		hFile					= INVALID_HANDLE_VALUE;
		
		file_no					= 0;

		InitializeCriticalSection(&multi_thread_lock);
	}

	void write(const char* content, ...)
	{
		CAutoCritical mutex_lock(&multi_thread_lock);

		if (check_file())
		{
			if (!create_log_file())
			{
				return;
			}
		}

		static char log_buffer[1024 * 100];
		DWORD dwIOByte = 0;

		if (write_time)		// 写入时间
		{
			time_t time_now = time(NULL);
			struct tm tm_now;
			localtime_s(&tm_now, &time_now);
			sprintf_s(log_buffer, sizeof(log_buffer), "%04d-%02d-%02d %02d:%02d:%02d : ",
				tm_now.tm_year + 1900,
				tm_now.tm_mon + 1,
				tm_now.tm_mday,
				tm_now.tm_hour,
				tm_now.tm_min,
				tm_now.tm_sec);

			//fwrite(log_buffer, 1, strlen(log_buffer), file_handle);			  		
			WriteFile(hFile, log_buffer, strlen(log_buffer), &dwIOByte, NULL);
		} 

		va_list arg_ptr;
		va_start(arg_ptr, content);
		vsprintf_s(log_buffer, content, arg_ptr);

		va_end(arg_ptr);

		strcat_s(log_buffer, "\r\n");			//增加一个换行

// 		fwrite(log_buffer, 1, strlen(log_buffer), file_handle);
// 		fflush(file_handle);
		WriteFile(hFile, log_buffer, strlen(log_buffer), &dwIOByte, NULL);
	}

	void close()
	{
// 		if (file_handle != NULL)
// 		{
// 			fclose(file_handle);
// 			file_handle = NULL;
// 		}
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
		}
	}

	void splitpath(const char* full_file_name, char* file_dir, char* file_name, char* file_ext_name)
	{
		strcpy_s(file_dir, 1024, full_file_name);

		char *p = file_dir;
		while (*(++p));

		while (*p != '/' && *p != '\\') 
			--p;

		++p;
		strcpy_s(file_name, 256, p);
		*p = '\0';						// 得到目录

		p = file_name;
		*file_ext_name = '\0';

		while (*(++p));

		while (p != file_name && *p != '.') 
			--p;

		if (p == file_name)		// 没有后缀 
		{
			return;
		}

		strcpy_s(file_ext_name, 64, p);	// 后缀
		*p = '\0';					// 文件名
	}

	bool create_log_file()		// 新建/打开日志文件
	{
		close();	

		char new_file_name[1204];

		if (!split_bydate && !split_bysize())	// 单日志模式
		{
			strcpy_s(new_file_name, log_file_name);
		} 
		else
		{
			char file_dir[1024], file_name[256], file_ext_name[64];
			splitpath(log_file_name, file_dir, file_name, file_ext_name);

			if (split_bydate)			// 按日期分割
			{
				if (split_bysize())
				{
					do					
					{
						sprintf_s(new_file_name, "%s%s_%04d%02d%02d_%d%s",
							file_dir,
							file_name,
							cur_date.year,
							cur_date.month,
							cur_date.day,
							file_no,
							file_ext_name);

						if (!is_file_exist(new_file_name))
						{
							break;
						}

						++file_no;
					} while(true);
				}
				else
				{
					sprintf_s(new_file_name, "%s%s_%04d%02d%02d%s", 
						file_dir,
						file_name,
						cur_date.year,
						cur_date.month,
						cur_date.day,
						file_ext_name);
				}
			}
			else
			{
				do
				{
					sprintf_s(new_file_name, "%s%s_%d%s",
						file_dir,
						file_name,
						file_no,
						file_ext_name);

					if (!is_file_exist(new_file_name))
					{
						break;
					}

					++file_no;
				} while (true);
			}
		}

		//fopen_s(&file_handle, new_file_name, "a");
		hFile = CreateFile(
							new_file_name,
							GENERIC_WRITE | GENERIC_READ,
							FILE_SHARE_READ,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL
							);

		m_strLastName = new_file_name;
		//return file_handle != NULL;
		return hFile != INVALID_HANDLE_VALUE;
	}

	const CString& GetLastFileName() { return m_strLastName; }

	bool is_file_exist(const char* file_name)
	{
		return _access(file_name, 0) != -1;
	}

	bool check_file()		// 检查是否需要新建文件
	{
		//if (file_handle == NULL)		// 文件没有打开
		if (hFile == INVALID_HANDLE_VALUE)		// 文件没有打开
		{
			return true;
		}

		if (split_bydate && cur_date.update_date())		// 日期变化
		{
			file_no = 0;
			return true;
		}

		if (split_bysize())		
		{
			//long file_length = ftell(file_handle) / 1024;
			DWORD file_length = GetFileSize(hFile, NULL) / 1024;

			if (file_length >= max_file_length)		// 超过最大限制
			{
				++file_no;
				return true;
			}
		}

		return false;
	}
protected:
	bool split_bysize() { return max_file_length > 0; }
private:
	//FILE * file_handle;
	HANDLE hFile;
	bool split_bydate;			// 是否按日生成
	DWORD max_file_length;		// 最大文件长度
	unsigned int file_no;		// 文件序号
	char log_file_name[1024];

	date_holder cur_date;

	bool write_time;		// 是否增加写入时间

	CString m_strLastName;	// 最近的文件名
	CRITICAL_SECTION	   multi_thread_lock;
};

class GWLogWriter : public static_singletonbase<GWLogWriter>
{
	IMPLEMENT_STATIC_SINGLETON_MODE(GWLogWriter)

public:
	~GWLogWriter(void);

	inline BOOL CheckLogLevel(GWLogLevel nLogLevel)
	{
		return m_nLogLevel > LogDisabled && nLogLevel >= m_nLogLevel;
	}

	void WriteLog(const CString& strMessage);

	inline const CString& GetLastLogFileName() 
	{ 
		return m_logWriter.GetLastFileName(); 
	}
private:
	void InitInstance();

private:
	GWLogLevel		m_nLogLevel;
	
	logwriter		m_logWriter;
};
