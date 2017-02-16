#include "StdAfx.h"
#include "SSLSocket.h"
static DWORD g_dwSocketIDAllocator = 100;

CTCPSocket::CTCPSocket(void)
{
	m_bConnect = FALSE;
	m_iPort = 0;
	m_dwSocketID = ++ g_dwSocketIDAllocator;
	m_dwUserType = 0;
}

CTCPSocket::~CTCPSocket(void)
{

}

BOOL CTCPSocket::SetNoBlock(bool bBlock)
{
	unsigned long ul = bBlock ? 0 : 1;
	map<int,SOCKET>::iterator it = m_Msocket.begin();
	for (;it!=m_Msocket.end();it++)
	{
		int ret = ioctlsocket(it->second,FIONBIO,(unsigned long*)&ul);
	}
	return TRUE;
}

BOOL CTCPSocket::IsConnected()
{
	return m_bConnect;
}

SOCKET CTCPSocket::GetSocket(int ilocal)
{
	return m_Msocket[ilocal];
}

DWORD CTCPSocket::GetSocketID()
{
	return m_dwSocketID;
}

DWORD CTCPSocket::GetUseType()
{
	return m_dwUserType;
}

BOOL CTCPSocket::ConnectServer(const char* strIP, int nPort,int iTotal)
{
	m_iPort = nPort;
	m_strIP = const_cast<char*>(strIP);
	//CloseSocket();
	int iSize = m_Msocket.size();
	for (int i = iSize;i<iSize+iTotal;i++)
	{
		m_Msocket[i] = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (SOCKET_ERROR == m_Msocket[i])
		{
			int nErr = ::GetLastError();
			CloseSocket();
			return FALSE;
		}
		SOCKADDR_IN addr;
		LPHOSTENT host = NULL;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(m_strIP);
		addr.sin_port = htons(m_iPort);
		if (connect(m_Msocket[i],(sockaddr *)&addr,sizeof(SOCKADDR_IN)))
		{
			int err = GetLastError();
			CloseSocket();
			return FALSE;
		}
		m_bConnect = TRUE;
	}
	return TRUE;
}

int CTCPSocket::Read(char* pszBuffer, int nBytes)
{
	if (pszBuffer == NULL || nBytes <= 0)
	{
		return 0;
	}
	int iAllReadBytes = 0;
	int nRemainLen  = nBytes;
	BOOL bErr = FALSE;
	while(1)
	{
		int nOnceReadLen = recv(m_Msocket[1],pszBuffer+iAllReadBytes,nRemainLen,0);
		if (nOnceReadLen <= 0)
		{
			int errorCode = WSAGetLastError();
			if (errorCode != WSAEINTR || errorCode != WSAEWOULDBLOCK)
			{
				bErr = TRUE;
			}
			else
			{
				Sleep(10);
			}
			break;
		}
		iAllReadBytes += nOnceReadLen;
		nRemainLen -= nOnceReadLen;
		if(nRemainLen <= 0)
		{
			break;
		}
	}
	if(bErr)
	{
		CloseSocket();
		return -1;
	}
	return iAllReadBytes;
}

int CTCPSocket::Send(char* pszBuffer, int nBytes)
{
	if(NULL == pszBuffer || nBytes <= 0)
	{
		return 0;
	}
	int nAllWriteByte = 0;
	int nRemainLen = nBytes;
	BOOL bErr = FALSE;
	while(1)
	{
		int nOnceWriteLen = send(m_Msocket[1], pszBuffer + nAllWriteByte, nRemainLen, 0); 
		if (nOnceWriteLen <= 0)
		{
			const int errorCode = WSAGetLastError();
			if (0!=errorCode || errorCode != WSAEINTR || errorCode != WSAEWOULDBLOCK)
			{ 
				bErr = 0;
				break;
			} 
			else
			{
				Sleep(10);
			}
		}
		nAllWriteByte += nOnceWriteLen;
		nRemainLen -= nOnceWriteLen;
		if(nRemainLen <= 0)
		{
			break;
		}
	}
	if(bErr)
	{
		CloseSocket();
		return -1;
	}
	return nAllWriteByte;
}

void CTCPSocket::CloseSocket()
{
	map<int,SOCKET>::iterator it = m_Msocket.begin();
	while(it!=m_Msocket.end())
	{
		if (it->second != INVALID_SOCKET)
		{
			closesocket(it->second);
			it->second = INVALID_SOCKET;
		}
		it++;
	}
	m_Msocket.clear();
	m_bConnect = FALSE;
}



CSSLSocket::CSSLSocket(void)
{
	// ³õÊ¼»¯  
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SSLeay_add_ssl_algorithms();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ERR_load_BIO_strings();

	m_pSSL = NULL;
	m_pSslCtx = NULL;


	m_pSslCtx = SSL_CTX_new(TLSv1_method());
	if (NULL == m_pSslCtx)
	{
		ASSERT(0);
	}
}

CSSLSocket::~CSSLSocket(void)
{
	CloseSocket();
	if (NULL != m_pSSL)
	{
		SSL_free(m_pSSL);
		m_pSSL = NULL;
	}
	if (NULL != m_pSslCtx)
	{
		SSL_CTX_free(m_pSslCtx);
		m_pSslCtx = NULL;
	}
}

BOOL CSSLSocket::ConnectServer(const char* strIP, int nPort,int iTotal)
{
// 	if (m_bConnect)
// 	{
// 		CloseSocket();
// 	}
	BOOL bRet = CTCPSocket::ConnectServer(strIP,nPort,iTotal);
	if (bRet == FALSE)
	{
		return FALSE;
	}
/*
	if (m_pSSL != NULL)
	{
		SSL_free(m_pSSL);
		m_pSSL = NULL;
	}
	m_pSSL = SSL_new(m_pSslCtx);
	if (m_pSSL == NULL)
	{
		CloseSocket();
		return FALSE;
	}
	int inet = SSL_set_fd(m_pSSL,m_socket);
	if (SSL_connect(m_pSSL) != 1)
	{
		CloseSocket();
		return FALSE;
	}*/
#if 0
	if (!SetNoBlock(false))
	{
		CloseSocket();
		return FALSE;
	}
#endif
	return TRUE;
}

int CSSLSocket::Read(SOCKET sock,char* pszBuffer, int nBytes ,int itype)
{
	if(NULL == pszBuffer || nBytes <= 0)
	{
		return 0;
	}
	if (itype == 0)
	{
		int nOnceReadLen = recv(sock,pszBuffer,nBytes,0);
		return nOnceReadLen;
	}
	else
	{
		int iAllReadBytes = 0;
		int nRemainLen  = nBytes;
		BOOL bErr = FALSE;
		while(1)
		{
			int nOnceReadLen = recv(sock,pszBuffer+iAllReadBytes,nRemainLen,0);
			if (nOnceReadLen <= 0)
			{
				int errorCode = WSAGetLastError();
				if (errorCode != WSAEINTR || errorCode != WSAEWOULDBLOCK)
				{
					bErr = TRUE;
				}
				else
				{
					Sleep(10);
				}
				break;
			}
			iAllReadBytes += nOnceReadLen;
			nRemainLen -= nOnceReadLen;
			if(nRemainLen <= 0)
			{
				break;
			}
		}
		if(bErr)
		{
			CloseSocket();
			return -1;
		}
		return iAllReadBytes;
	}
	return  0;



// 	int nAllReadByte = 0;
// 	int nRemainLen = nBytes;
// 	BOOL bErr = FALSE;
// 	while(1)
// 	{
// 		int nOnceReadLen = SSL_read(m_pSSL, pszBuffer + nAllReadByte, nRemainLen);
// 		int errorCode = SSL_get_error(m_pSSL, nOnceReadLen);
// 		switch (errorCode)
// 		{
// 		case SSL_ERROR_ZERO_RETURN:
// 		case SSL_ERROR_SYSCALL:
// 			bErr = TRUE;
// 			break;
// 		case SSL_ERROR_NONE:
// 			break;
// 		case SSL_ERROR_WANT_READ:
// 		case SSL_ERROR_WANT_WRITE:
// 			Sleep(10); //block ×´Ì¬
// 		default:
// 			break;
// 		}
// 		if(bErr)
// 		{
// 			CloseSocket();
// 			return -1;
// 		}
// 		nAllReadByte += nOnceReadLen;
// 		nRemainLen -= nOnceReadLen;
// 		if(nRemainLen <= 0)
// 		{
// 			break;
// 		}
// 	}
// 	return nAllReadByte;
}

int CSSLSocket::Send(char* pszBuffer, int nBytes)
{
	if(NULL == pszBuffer || nBytes <= 0)
	{
		return 0;
	}
	map<int,SOCKET>::iterator it = m_Msocket.begin();
	while (it!=m_Msocket.end())
	{
		int nOnceWriteLen = send(it->second, pszBuffer, nBytes, 0); 
		if (nOnceWriteLen == SOCKET_ERROR)
		{
			closesocket(it->second);
			it->second = INVALID_SOCKET;
			it = m_Msocket.erase(it);
			continue;
		}
		TRACE("send->socket:%d,text:%s\n",it->second,pszBuffer+52);
		it++;
	}
	return 0;
// 	int nAllWriteByte = 0;
// 	int nRemainLen = nBytes;
// 	BOOL bErr = FALSE;
// 	while(1)
// 	{
// 		int nOnceWriteLen = SSL_write(m_pSSL, pszBuffer + nAllWriteByte, nRemainLen);
// 		int errorCode = SSL_get_error(m_pSSL, nOnceWriteLen);
// 		switch (errorCode)
// 		{
// 		case SSL_ERROR_ZERO_RETURN:
// 		case SSL_ERROR_SYSCALL:
// 			bErr = TRUE;
// 			break;
// 		case SSL_ERROR_NONE:
// 			break;
// 		case SSL_ERROR_WANT_READ:
// 		case SSL_ERROR_WANT_WRITE:
// 			Sleep(10); //block ×´Ì¬
// 		default:
// 			break;
// 		}
// 		if(bErr)
// 		{
// 			CloseSocket();
// 			return -1;
// 		}
// 		nAllWriteByte += nOnceWriteLen;
// 		nRemainLen -= nOnceWriteLen;
// 		if(nRemainLen <= 0)
// 		{
// 			break;
// 		}
// 	}
// 	return nAllWriteByte;
}
