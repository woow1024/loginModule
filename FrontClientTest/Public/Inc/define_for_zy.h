#pragma once

/************************************************************************/
/*  2014-10-07 中宇版专用头文件                                         */
/************************************************************************/

#ifndef VERSION_TAG_ZY
#error 引用了中宇版头文件却没有定义VERSION_TAG_ZY宏，无法继续编译
#endif

#define VERSION_ACCOUNTTYPE									7	
#define VERSION_TAG											_T("TY")			
#define GW_VERSION_NAME										_T("银天下推广")//2015-5-19推广版修改 _T("中宇金业")

#define REMOTE_UPDATE_DIR									_T("/upgrade/ty/")																				// 升级下载目录
#define REMOTE_JIAOYI_DIR									_T("jiaoyi/ty")																					// 交易端下载目录

#define VERSION_SERVICE_PHONENUM							_T("400-115-1155")																				// 中宇联系电话
#define VERSION_TAG_CODE									_T("TY")																						// 中宇代码
#define GWSTATION_UPGRADE_SEM								_T("__@@GWSTATION_@@_UPGRADE@@@__TY_VERSION__")													// 更新信号量
#define GWSTATION_APP_SEM							_T("{B7B29D35-623C-4e75-B768-2CE130DC203B}")													// 单实例内核对象


#define MH_LOGIN_UI_TIP										_T("")//2015-5-19推广版修改	_T("您专属的贵金属投资顾问")
#define MH_LOGIN_UI_TITLE									_T("银天下登录")
#define FRAMEWINDOW_TITLE									_T("银天下行情分析系统")
#define THANKS_USE_TIP										_T("再次感谢您对银天下的支持！")
#define REFFERAL_LINK_TIP									_T("推荐链接复制成功，请粘贴到您的QQ/微博上，推荐给您的好友！再次感谢您对银天下的支持！")
#define REFFERAL_CLIPBOARD_TIP							_T(" 贵金属投资用啥软件？必备利器——银天下！速度快，系统稳，功能强！即刻免费使用%s")

#define ENABLE_SIMULATED_TRADE								// 模拟交易