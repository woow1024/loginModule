#pragma once

#include <vector>
#include <list>
#include <map>
#include "GWDefine.h"

using namespace std;
#pragma pack(push,1)
struct tagTTPacketMarketHead
{
// 	unsigned char  bStartFlag;     // Э�����ʼ��־ 0xFF
// 	unsigned char  bVer;           // �汾��
// 	unsigned char  bEncryptFlag;   // ���ܱ�־(���������,��Ϊ0)
// 	unsigned char  bFrag;          // �Ƿ��а���Ƭ(1:�а���Ƭ 0:�ް���Ƭ)
// 	unsigned short  wLen;          // �ܰ��� (�����ֽ���)
// 	unsigned short  wCmd;          // ����� (�����ֽ���)
// 	unsigned short  wSeq;          // �������к�,ҵ��ʹ��
// 	unsigned short  wCrc;          // Crc16У����
// 	unsigned int    dwSID;         // �ỰID
// 	unsigned short  wTotal;        // �а���Ƭʱ����Ƭ����
// 	unsigned short  wCurSeq;       // �а���Ƭʱ����Ƭ���
	DWORD dwTotalLen;	//���峤�ȣ�����head
	byte byStartFlag;	//must be 0x7F
	byte uCC;	//CC flag
	byte uZip;	//ѹ����ʽ  0 ��ѹ��  1 ZIP 
	byte bFrag;	//�Ƿ�ְ�
	short wTotalPacket;	//������
	short wPacketIndex;	//��ǰ������
	DWORD dwSeqID;	//��ˮID
	DWORD dwCmd;	//command ID
	DWORD dwTimeStamp;	//ʱ���
	DWORD dwAppendData;		//�ش��ֶ�
	int  dwFromSocket;
	int dwReserved;
	//char szFrom[8];	//��Դ
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
