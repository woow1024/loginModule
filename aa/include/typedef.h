#pragma once

#ifdef _WIN32
#include <WTypes.h>

#else
#include <string.h>
typedef unsigned int DWORD;
typedef long INT64;
typedef bool  BOOL;
typedef int  SOCKET;
typedef  unsigned char BYTE;
typedef  unsigned char byte;
#define  NULL		0
#define	FALSE		0
#define TRUE		1 

#define SOCKET_ERROR (-1)

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif
