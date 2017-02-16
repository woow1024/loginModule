#pragma once

#include <vector>
#include <list>
#include <map>
#include "GWDefine.h"

using namespace std;
#pragma pack(push,1)
struct tagTTPacketMarketHead
{
// 	unsigned char  bStartFlag;     // 协议包起始标志 0xFF
// 	unsigned char  bVer;           // 版本号
// 	unsigned char  bEncryptFlag;   // 加密标志(如果不加密,则为0)
// 	unsigned char  bFrag;          // 是否有包分片(1:有包分片 0:无包分片)
// 	unsigned short  wLen;          // 总包长 (网络字节序)
// 	unsigned short  wCmd;          // 命令号 (网络字节序)
// 	unsigned short  wSeq;          // 包的序列号,业务使用
// 	unsigned short  wCrc;          // Crc16校验码
// 	unsigned int    dwSID;         // 会话ID
// 	unsigned short  wTotal;        // 有包分片时，分片总数
// 	unsigned short  wCurSeq;       // 有包分片时，分片序号
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

struct Market
{
	tagTTPacketMarketHead tPackeHead;
	char * pszBody;

// 	DWORD dwFromSocket;
// 	DWORD dwToSocket;
// 	DWORD dwAppendData;
// 	DWORD dwReserved[2];

	Market()
	{
		memset(this, 0, sizeof(Market));
	}
	~Market()
	{
		if(pszBody)
		{
			delete [] pszBody;
		}
	}
};
#pragma pack(pop)

DECLARE_VECTOR(Market*, GWTTPacket)
DECLARE_LIST(Market*, GWTTPacket)
DECLARE_MAP(int, Market*, GWTTPacket)

struct tagRspMarketHead
{
// 	int ret;
// 	CString msg;
// 	CString uname;
// 	CString yg_trade_key;
	int iFrom;
	int iSeq;
	BOOL bSuccess;
	int iCmd;
	int iRet;
	string sMsg;

};

struct tagMarketInfoRspMarket
{
	int Id;
};
DECLARE_MAP(int,tagMarketInfoRspMarket*,tagMarketInfoRspMarketPtr)
