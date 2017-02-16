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
protected:
	SOCKET m_socket;
	int m_iPort;
	char* m_strIP;
	BOOL m_bConnect;
	DWORD m_dwSocketID;
	DWORD m_dwUserType;

public:
	BOOL SetNoBlock(bool bBlock);//��������ģʽ 1������ 0����
	BOOL IsConnected();
	SOCKET GetSocket();
	DWORD GetSocketID();
	DWORD GetUseType();

	virtual BOOL ConnectServer(const char* strIP, int nPort);
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
	virtual BOOL ConnectServer(const char* strIP, int nPort);
	virtual int Read(char* pszBuffer, int nBytes ,int itype);
	virtual int Send(char* pszBuffer, int nBytes);
};