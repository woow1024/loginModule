#pragma once

/************************************************************************/
/*  2015-11-23 金兜版专用头文件                                     */
/************************************************************************/

#ifndef VERSION_TAG_JD
#error 引用了北石油版头文件却没有定义VERSION_TAG_BSY宏，无法继续编译
#endif

#define VERSION_ACCOUNTTYPE								11	
#define VERSION_TAG										_T("JD")			
#define GW_VERSION_NAME									_T("银天下|金兜信息技术有限公司")

#define REMOTE_UPDATE_DIR								_T("/upgrade/jd/")																				// 升级下载目录
#define REMOTE_JIAOYI_DIR								_T("jiaoyi/jd")																					// 交易端下载目录

#define VERSION_SERVICE_PHONENUM						_T("400-1677-003")																					// 金兜版联系电话
#define VERSION_TAG_CODE								_T("JD")																							// 北石油版代码
#define GWSTATION_UPGRADE_SEM							_T("__@@GWSTATION_@@_UPGRADE@@@__JD_VERSION__")													// 更新信号量
#define GWSTATION_APP_SEM							_T("{D529D23F-1955-4522-A11B-FAFAFFA031C5}")													// 单实例内核对象

#define MH_LOGIN_UI_TIP									_T("")
#define MH_LOGIN_UI_TITLE								_T("金兜财富登录")
#define FRAMEWINDOW_TITLE								_T("金兜财富行情分析系统")
#define THANKS_USE_TIP									_T("再次感谢您对银天下|金兜的支持！")
#define REFFERAL_LINK_TIP								_T("推荐链接复制成功，请粘贴到您的QQ/微博上，推荐给您的好友！再次感谢您对银天下|金兜的支持！")
#define REFFERAL_CLIPBOARD_TIP							_T(" 黄金投资哪家强？投资交易找金兜！行情、直播、策略应有尽有！即刻免费使用http://www.jdcf88.com/%s")

#define ENABLE_ENTRUST_TRADE							// 开启实盘交易
#define ENABLE_SIMULATED_TRADE							// 模拟交易

#define ENABLE_AD										// 广告