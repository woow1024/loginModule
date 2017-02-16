#pragma once


#define  WM_WEBKIT_MSG_BASE						(WM_USER + 333)
#define  WM_NOTIFY_BROSER_CREATE_OK				(WM_WEBKIT_MSG_BASE + 1)		//browser ����״̬
#define  WM_NOTIFY_TITLE_CHANGE					(WM_WEBKIT_MSG_BASE + 2)		//�������ı�
#define  WM_NOTIFY_URL_CHANGE					(WM_WEBKIT_MSG_BASE + 3)		//URL �ı�
#define  WM_NOTIFY_NAVIGATE_STATUS_CHANGE		(WM_WEBKIT_MSG_BASE + 4)		//����״̬
#define  WM_NOTIFY_DOWNLOAD_STATUS_CHANGE		(WM_WEBKIT_MSG_BASE + 5)		//����״̬
#define  WM_NOTIFY_POPUP_URL					(WM_WEBKIT_MSG_BASE + 10)


enum  EM_DOWNLOAD_STATUS
{
	em_Download_Idle = 0,
	em_Download_Begin,
	em_Download_InProcess,
	em_Download_End,
	em_Download_Error,
};