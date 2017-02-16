

#ifndef __DISPATCHERMESSAGE_H
#define __DISPATCHERMESSAGE_H 1

#ifdef DISPATCHERCLIENT_EXPORTS
#define DISPATCHERCLIENT_API __declspec(dllexport)
#else
#if defined(LINUX) || defined(MINGW) || defined(MINGW32)
#define DISPATCHERCLIENT_API
#else
#define DISPATCHERCLIENT_API __declspec(dllimport)
#endif
#endif


#include "DispatcherInt.h"
#include <string>
#include <sstream>
//#include "DispatcherClientConf.h"

/*
	为了编写简单，不得己将内部类给暴露出来，未来将修复这个问题
*/
namespace tt{
	class message2 ;
}
/*
	消息类，不可继承。由动态库内部实现。
*/
class DISPATCHERCLIENT_API DispatcherMessage {
private:
	tt::message2 * message_ ;
	std::string subject_name_ ;
	int subject_code_ ;
public:	
	static void Init();
	static void UnInit();
	DispatcherMessage() ;
	~DispatcherMessage() ;
	
	/*
		提取概要信息，包括主题名，该主题包含的字段数。
		每个字段的名字和类型
	*/
	const std::string& GetSubjectName() const;
	int GetSubjectType() const;
	int SetSubjectType(int type);
	int GetFieldCount() const;
	void Print(std::ostringstream& os);
	/*
		按名提取每个字段的值。如果该字段名是有效，那么返回true。
		如果名字和类型不匹配，则返回false。注意，有符号和无符号是不兼容的。
	*/
	bool GetFieldInt32(const char *name , int32& value) const;
	bool GetFieldInt64(const char *name , int64& value) const;
	bool GetFieldUInt32(const char *name , uint32& value) const;
	bool GetFieldUInt64(const char *name , uint64& value) const;
	bool GetFieldDouble(const char *name , double& value) const;
	bool GetFieldFloat(const char *name , float& value) const;
	bool GetFieldBool(const char *name , bool& value) const;
	bool GetFieldTimeStamp(const char *name , int64& value) const;
	bool GetFieldString(const char *name ,char* buffer , int len) const;
	bool GetFieldRaw(const char *name ,char* buffer , int len) const;

	/*
		设置主题名，格式为TIBCO主题格式相同
	*/
	void SetSubjectName(const char *name) ;
	
	/*
		按名添加字段值，不提供按索引的方式
	*/
	bool AddFieldInt32(const char *name , int32 value) ;
	bool AddFieldInt64(const char *name , int64 value) ;
	bool AddFieldUInt32(const char *name , uint32 value) ;
	bool AddFieldUInt64(const char *name , uint64 value) ;
	bool AddFieldDouble(const char *name , double value) ;
	bool AddFieldFloat(const char *name , float value) ;
	bool AddFieldBool(const char *name , bool value) ;
	bool AddFieldTimeStamp(const char *name , int64 value) ;
	bool AddFieldString(const char *name , const char* buffer) ;

	void AttachMessage(tt::message2 * message = NULL) ;
	tt::message2 * AttachedMessage() ;
	tt::message2 * CreateMsg();
	void reset();
	//int get_size() const;
	static DispatcherMessage *Create() ;
	static void Destory(DispatcherMessage *& p);
} ;


class SafeMessageQueueAdapter ;

class DISPATCHERCLIENT_API SafeMessageQueue {
private:
	SafeMessageQueueAdapter * adapter_ ;
public:
	SafeMessageQueue() ;
	~SafeMessageQueue() ;

	DispatcherMessage * dequeue(int timeout = 1) ;
	void enqueue(DispatcherMessage * message) ;
	void clear() ;
	int count() const;
} ;


#endif /** __DISPATCHERMESSAGE_H */


