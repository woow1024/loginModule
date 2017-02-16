#pragma once
#include "gwtypes.h"
#include "gwinterface.h"
#include "autoobj.h"

class CUIRacketCtrl
{
public:
	CUIRacketCtrl(void);
	~CUIRacketCtrl(void);
};

class CMultPacketCtrl: public CCriticalSectionBase
{
	DECLARE_MAP(INT32, GWTTPacketVEC*, MultPacket)

public:
	CMultPacketCtrl();
	~CMultPacketCtrl();
private:
	MultPacketMAP m_cMultTTPacket;
public:
	Market * MergePacket(Market * pTTPacket);
	void Clear();
private:
	GWTTPacketVEC* GetPacketList(int nSeqID);
	void RemovePacketList(int nSeqID);
};

void N2HPacket(Market * & pOnePacket, BOOL bN2H = TRUE);
string UTF8ToGB(const char* str);
string GBToUTF8(const char* str);