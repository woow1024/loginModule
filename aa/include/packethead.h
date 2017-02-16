#ifndef	_STRUCT_PACKEET_HEADER_
#define	_STRUCT_PACKEET_HEADER_

#include "typedef.h"

#pragma  pack (push,1)


struct tagPacketHead
{
	DWORD dwTotalLen;	//包体长度，包含head
	byte byStartFlag;	//must be 0x7F
	byte uCC;	//CC flag
	byte uZip;	//压缩方式  0 不压缩  1 ZIP 
	byte bFrag;	//是否分包
	short wTotalPacket;	//包总数
	short wPacketIndex;	//当前包索引
	DWORD dwSeqID;	//流水ID
	DWORD dwCmd;	//command ID
	DWORD dwTimeStamp;	//时间戳
	DWORD dwAppendData;		//回传字段
	int  dwFromSocket;
	int dwReserved;
	//char szFrom[8];	//来源
	BYTE byReserved[16];	
};

struct tagPacket
{
	tagPacketHead tPackeHead;
	char  szBody; 
};


#define  WX_CMD_AUTHOR_REQ		999  //授权请求

#define  WX_CMD_LOGIN_REQ		2000	//登录请求


#pragma pack(pop)
#endif // _STD_HELPER_MACRO_H