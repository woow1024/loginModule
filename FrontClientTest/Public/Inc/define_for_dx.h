#pragma once

/************************************************************************/
/*  2015-08-17 大象金服版专用头文件                                         */
/************************************************************************/

#ifndef VERSION_TAG_DX
#error 引用了大象金服版头文件却没有定义VERSION_TAG_DX宏，无法继续编译
#endif

#define VERSION_ACCOUNTTYPE									10 
#define VERSION_TAG											_T("DX")			
#define GW_VERSION_NAME										_T("大象金服")

#define REMOTE_UPDATE_DIR									_T("/upgrade/dx/")																				// 升级下载目录
#define REMOTE_JIAOYI_DIR									_T("jiaoyi/dx")																					// 交易端下载目录

#define VERSION_SERVICE_PHONENUM							_T("400-115-1155")																				// 大象金服联系电话
#define VERSION_TAG_CODE									_T("DX")																						// 大象金服代码
#define GWSTATION_UPGRADE_SEM								_T("__@@GWSTATION_@@_UPGRADE@@@__DX_VERSION__")													// 更新信号量
#define GWSTATION_APP_SEM							_T("{31178456-8D9B-439d-AC41-DB627017ADD0}")													// 单实例内核对象


#define MH_LOGIN_UI_TIP										_T("")
#define MH_LOGIN_UI_TITLE									_T("大象金服")
#define FRAMEWINDOW_TITLE									_T("大象金服行情分析系统")
#define THANKS_USE_TIP										_T("再次感谢您对大象金服的支持！")
#define REFFERAL_LINK_TIP									_T("推荐链接复制成功，请粘贴到您的QQ/微博上，推荐给您的好友！再次感谢您对大象金服的支持！")
#define REFFERAL_CLIPBOARD_TIP							_T(" 现货投资用啥软件？必备利器——大象金服！速度快，系统稳，功能强！即刻免费使用%s")


#define ENABLE_ENTRUST_TRADE							// 开启实盘交易
#define ENABLE_SIMULATED_TRADE								// 模拟交易