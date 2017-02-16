#pragma once

/************************************************************************/
/*  2014-10-07 兴业银行版专用头文件                                     */
/************************************************************************/

#ifndef VERSION_TAG_XYYH
#error 引用了兴业银行版头文件却没有定义VERSION_TAG_XYYH宏，无法继续编译
#endif

#define VERSION_ACCOUNTTYPE								5	
#define VERSION_TAG										_T("XYYH")			
#define GW_VERSION_NAME									_T("兴业银行")
#define REMOTE_UPDATE_DIR								_T("/upgrade/xyyh/")																				// 升级下载目录
#define REMOTE_JIAOYI_DIR								_T("jiaoyi/xyyh")																					// 交易端下载目录

#define VERSION_SERVICE_PHONENUM						_T("4008-213-988")																					// 兴业银行版联系电话
#define VERSION_TAG_CODE								_T("XYYH")																							// 兴业银行版代码
#define GWSTATION_UPGRADE_SEM							_T("__@@GWSTATION_@@_UPGRADE@@@__XYYH_VERSION__")													// 更新信号量
#define GWSTATION_APP_SEM							_T("{F757B531-1F65-4f02-B760-ECCF667C961F}")													// 单实例内核对象

#define MH_LOGIN_UI_TIP									_T("您专属的黄金交易专家")
#define MH_LOGIN_UI_TITLE								_T("兴业银行登录")
#define FRAMEWINDOW_TITLE								_T("兴业银行行情分析系统")
#define THANKS_USE_TIP									_T("再次感谢您对兴业银行的支持！")
#define REFFERAL_LINK_TIP								_T("推荐链接复制成功，请粘贴到您的QQ/微博上，推荐给您的好友！再次感谢您对兴业银行的支持！")
#define REFFERAL_CLIPBOARD_TIP							_T(" 贵金属投资用啥软件？必备利器——银天下！速度快，系统稳，功能强！即刻免费使用%s")

#define ENABLE_ENTRUST_TRADE							// 开启实盘交易
#define ENABLE_SIMULATED_TRADE							// 模拟交易