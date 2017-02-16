#pragma once

typedef enum enumUserOperationType
{
	Change_View	= 0,
	Change_Symbol,
	Change_URL

} UserOperationType;

#define COMMON_OP	0
#define MENU_OP		1
#define TOOLBAR_OP	2
#define KEYBOARD_OP	3

typedef struct _USEROPERATION
{
	UserOperationType op_type;
	TCHAR user[48];
	time_t time;
	TCHAR from[16];
	TCHAR to[16];
	TCHAR symbol[32];
	TCHAR key[16];
	USHORT type;
} USEROPERATION, *LPUSEROPERATION;