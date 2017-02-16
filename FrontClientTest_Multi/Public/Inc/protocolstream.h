#pragma once

#include <stdlib.h>
#include <string>
#include <sstream>
//#include<stdint.h>
#include "TTNetIOConf.h"
#include "TTInt.h"
#if defined(LINUX)
#include <sys/types.h>
#include <netinet/in.h>
#else
#include <WinSock2.h>
#pragma comment(lib,"WS2_32.lib")
typedef unsigned int uint32_t;
typedef uint32_t u_int32_t;
typedef int socklen_t;
#endif

//二进制协议的打包解包类，内部的服务器之间通讯，统一采用这些类
enum {
	TEXT_PACKLEN_LEN	= 4,
	TEXT_PACKAGE_MAXLEN	= 0xffff,
	BINARY_PACKLEN_LEN	= 2,
	BINARY_PACKAGE_MAXLEN	= 0xffff,

	TEXT_PACKLEN_LEN_2	= 6,
	TEXT_PACKAGE_MAXLEN_2	= 0xffffff,

	BINARY_PACKLEN_LEN_2 	= 4,//4字节头长度
	BINARY_PACKAGE_MAXLEN_2 = 0x10000000,//包最大长度是256M,足够了

	CHECKSUM_LEN		= 2,
};

//计算校验和
unsigned short checksum(const unsigned short *buffer,int size);
bool compress(uint32_t i, char *buf, size_t &len);
bool uncompress(char *buf, size_t len, uint32_t &i);

struct TTNETIO_API ReadStreamImpl
{
	virtual const char* GetData() const = 0;
	virtual size_t GetSize() const = 0;
};

struct TTNETIO_API WriteStreamImpl
{
	virtual const char* GetData() const = 0;
	virtual size_t GetSize() const = 0;
};
class TTNETIO_API BinaryReadStream2 : public ReadStreamImpl
{
	private:
		const char* const ptr;
		const size_t len;
		const char* cur;
		BinaryReadStream2(const BinaryReadStream2&);
		BinaryReadStream2& operator=(const BinaryReadStream2&);
	public:
		BinaryReadStream2(const char* ptr, size_t len);
		//bool CheckSum();
		virtual const char* GetData() const;
		virtual size_t GetSize() const;
		bool IsEmpty() const;
		bool Read(std::string *str,size_t maxlen,size_t& outlen);
		bool Read(char* str,size_t strlen,size_t& len);
		bool Read(const char** str,size_t maxlen,size_t& outlen);
		bool Read(int &i);
		bool Read(short &i);
		bool Read(char &c);
		size_t ReadAll(char * szBuffer, size_t iLen) const;
		bool IsEnd() const;
		const char* GetCurrent() const{ return cur; }

	public:
		bool ReadLength(size_t & len);
		bool ReadLengthWithoutOffset(size_t &headlen, size_t & outlen);
};
class TTNETIO_API BinaryWriteStream2 : public WriteStreamImpl
{
	protected:
		char* const ptr;
		const size_t len;
		char*  cur;
		BinaryWriteStream2(const BinaryWriteStream2&);
		BinaryWriteStream2& operator=(const BinaryWriteStream2&);
	public:
		BinaryWriteStream2(char* ptr, size_t len);
		virtual const char* GetData() const;
		virtual size_t GetSize() const;
		bool Write(const char* str, size_t len);
		bool Write(int i);
		bool Write(short i);
		bool Write(char c);
		bool IsValid() const;
		//void CheckSum();
		void Flush();
		void Clear();				
		char* GetCurrent() const{ return cur; }
		bool Skip(size_t len_);
	public:
		bool WriteLength(size_t len);
};
class TTNETIO_API BinaryWriteStream3 : public WriteStreamImpl
{
	public:
		BinaryWriteStream3(std::string *data);
		virtual const char* GetData() const;
		virtual size_t GetSize() const;
		bool Write(const char* str,size_t len);
		bool  Write(double value,bool isNULL = false);
		bool  Write(tt::int64  value,bool isNULL = false);
		bool Write(int i,bool isNULL = false);
		bool Write(short i,bool isNULL = false);
		bool Write(char c,bool isNULL = false);
		//void CheckSum(char *ptr);
		size_t GetCurrentPos() const{return m_data->length();}
		void Flush();
		void Clear();
	private:
		std::string *m_data;
};
