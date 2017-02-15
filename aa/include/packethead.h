#ifndef	_STRUCT_PACKEET_HEADER_
#define	_STRUCT_PACKEET_HEADER_

#include "typedef.h"

#pragma  pack (push,1)


struct tagPacketHead
{
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

struct tagPacket
{
	tagPacketHead tPackeHead;
	char  szBody; 
};


#define  WX_CMD_AUTHOR_REQ		999  //��Ȩ����

#define  WX_CMD_LOGIN_REQ		2000	//��¼����


#pragma pack(pop)
#endif // _STD_HELPER_MACRO_H