#include "DCManager.h"
#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#endif
#include <iostream>
using namespace std;

static BOOL m_bRunFlag = TRUE;


void handle_signal(int signal_no)
{
	if (signal_no == SIGINT)
	{
		m_bRunFlag = FALSE;
		CDCManager::GetInstance()->Stop();
		CDCManager::GetInstance()->join_all_thread();
	}
}


int main(int argc,char *argv[])
{
	BOOL bRet = CDCManager::GetInstance()->ReadyConnect();
	if(FALSE == bRet)
	{
		exit(0);
	}

	signal(SIGINT, handle_signal);	

	CDCManager::GetInstance()->Start();
	while(m_bRunFlag)
	{

#ifdef _WIN32
		Sleep(1000);
#else
		usleep(1000 * 1000);
#endif 
	}
	return 0;
}