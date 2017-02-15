#include "StdAfx.h"
#include "UIRacketCtrl.h"

CUIRacketCtrl::CUIRacketCtrl(void)
{
}

CUIRacketCtrl::~CUIRacketCtrl(void)
{
}

void N2HPacket(Market * & pOnePacket, BOOL bN2H /*= TRUE*/)
{
// 	if(NULL == pOnePacket)
// 	{
// 		return;
// 	}
// 	tagTTPacketMarketHead & tPacketHead = pOnePacket->tPackeHead;
// 	if(bN2H)
// 	{
// 		tPacketHead.wLen = ntohs(tPacketHead.wLen);
// 		tPacketHead.wCmd = ntohs(tPacketHead.wCmd);
// 		tPacketHead.wSeq = ntohs(tPacketHead.wSeq);
// 		tPacketHead.wCrc = ntohs(tPacketHead.wCrc);
// 		tPacketHead.dwSID = ntohl(tPacketHead.dwSID);
// 		tPacketHead.wTotal = ntohs(tPacketHead.wTotal);
// 		tPacketHead.wCurSeq = ntohs(tPacketHead.wCurSeq);
// 	}
// 	else
// 	{
// 		tPacketHead.wLen = htons(tPacketHead.wLen);
// 		tPacketHead.wCmd = htons(tPacketHead.wCmd);
// 		tPacketHead.wSeq = htons(tPacketHead.wSeq);
// 		tPacketHead.wCrc = htons(tPacketHead.wCrc);
// 		tPacketHead.dwSID = htonl(tPacketHead.dwSID);
// 		tPacketHead.wTotal = htons(tPacketHead.wTotal);
// 		tPacketHead.wCurSeq = htons(tPacketHead.wCurSeq);
// 
// 	}

	if(NULL == pOnePacket)
	{
		return;
	}

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
	tagTTPacketMarketHead & tPacketHead = pOnePacket->tPackeHead;
	if(bN2H)
	{
		tPacketHead.wTotalPacket = ntohs(tPacketHead.wTotalPacket);
		tPacketHead.wPacketIndex = ntohs(tPacketHead.wPacketIndex);
		tPacketHead.dwTotalLen = ntohl(tPacketHead.dwTotalLen);
		tPacketHead.dwCmd = ntohl(tPacketHead.dwCmd);
		tPacketHead.dwTimeStamp = ntohl(tPacketHead.dwTimeStamp);
		tPacketHead.dwAppendData = ntohl(tPacketHead.dwAppendData);
		tPacketHead.dwReserved = ntohl(tPacketHead.dwReserved);

	}
	else
	{
		tPacketHead.wTotalPacket = htons(tPacketHead.wTotalPacket);
		tPacketHead.wPacketIndex = htons(tPacketHead.wPacketIndex);
		tPacketHead.dwTotalLen = htonl(tPacketHead.dwTotalLen);
		tPacketHead.dwCmd = htonl(tPacketHead.dwCmd);
		tPacketHead.dwTimeStamp = htonl(tPacketHead.dwTimeStamp);
		tPacketHead.dwAppendData = htonl(tPacketHead.dwAppendData);
		tPacketHead.dwReserved = htonl(tPacketHead.dwReserved);

	}
}

std::string UTF8ToGB(const char* str)
{
	std::string result;
	WCHAR *strSrc;
	TCHAR *szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i+1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
}

std::string GBToUTF8(const char* str)
{
	std::string result;
	WCHAR *strSrc;
	TCHAR *szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new TCHAR[i+1];
	WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
}

Market * CMultPacketCtrl::MergePacket(Market * pTTPacket)
{
	return NULL;
}

void CMultPacketCtrl::Clear()
{

}

GWTTPacketVEC* CMultPacketCtrl::GetPacketList(int nSeqID)
{
	return NULL;
}

void CMultPacketCtrl::RemovePacketList(int nSeqID)
{

}
