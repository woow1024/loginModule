

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
	Ϊ�˱�д�򵥣����ü����ڲ������¶������δ�����޸��������
*/
namespace tt{
	class message2 ;
}
/*
	��Ϣ�࣬���ɼ̳С��ɶ�̬���ڲ�ʵ�֡�
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
		��ȡ��Ҫ��Ϣ��������������������������ֶ�����
		ÿ���ֶε����ֺ�����
	*/
	const std::string& GetSubjectName() const;
	int GetSubjectType() const;
	int SetSubjectType(int type);
	int GetFieldCount() const;
	void Print(std::ostringstream& os);
	/*
		������ȡÿ���ֶε�ֵ��������ֶ�������Ч����ô����true��
		������ֺ����Ͳ�ƥ�䣬�򷵻�false��ע�⣬�з��ź��޷����ǲ����ݵġ�
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
		��������������ʽΪTIBCO�����ʽ��ͬ
	*/
	void SetSubjectName(const char *name) ;
	
	/*
		��������ֶ�ֵ�����ṩ�������ķ�ʽ
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


