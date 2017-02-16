#pragma once

#include "ssl.h"
#include "x509.h"
#include "rand.h"
#include "err.h"
#include "bio.h" 
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <istream>
#include <map>

enum LoginStatus
{
	LOGINSTATUS_LOGINING = -2,
	LOGINSTATUS_LOGOUT,
	LOGINSTATUS_EXCHANGEID,
	LOGINSTATUS_DATACENTER
};
class CTCPSocket
{
public: 
	CTCPSocket(void);
	virtual ~CTCPSocket(void);
public:
	map<int,SOCKET> m_Msocket;
protected:
	int m_iPort;
	char* m_strIP;
	BOOL m_bConnect;
	DWORD m_dwSocketID;
	DWORD m_dwUserType;

public:
	BOOL SetNoBlock(bool bBlock);//设置阻塞模式 1非阻塞 0阻塞
	BOOL IsConnected();
	SOCKET GetSocket(int ilocal);
	DWORD GetSocketID();
	DWORD GetUseType();

	virtual BOOL ConnectServer(const char* strIP, int nPort,int iTotal);
	virtual int Read(char* pszBuffer, int nBytes);
	virtual int Send(char* pszBuffer, int nBytes);
	virtual void CloseSocket();
};



class CSSLSocket : public CTCPSocket
{
public:
	CSSLSocket(void);
	virtual ~CSSLSocket(void);
private:
	SSL_CTX* m_pSslCtx;
	SSL*	m_pSSL;
public:
	virtual BOOL ConnectServer(const char* strIP, int nPort,int iTotal);
	virtual int Read(SOCKET sock,char* pszBuffer, int nBytes ,int itype);
	virtual int Send(char* pszBuffer, int nBytes);
};
