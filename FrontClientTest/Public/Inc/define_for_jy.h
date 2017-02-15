#pragma once

/************************************************************************/
/*  2015-11-23 金异版专用头文件                                     */
/************************************************************************/

#ifndef VERSION_TAG_JY
#error 引用了金异版头文件却没有定义VERSION_TAG_JY宏，无法继续编译
#endif

#define VERSION_ACCOUNTTYPE								13	
#define VERSION_TAG										_T("JY")			
#define GW_VERSION_NAME									_T("上海金异信息技术有限公司")

#define REMOTE_UPDATE_DIR								_T("/upgrade/jy/")																				// 升级下载目录
#define REMOTE_JIAOYI_DIR								_T("jiaoyi/jy")																					// 交易端下载目录

#define VERSION_SERVICE_PHONENUM						_T("021-31587778")																					// 金兜版联系电话
#define VERSION_TAG_CODE								_T("JY")																							// 北石油版代码
#define GWSTATION_UPGRADE_SEM							_T("__@@GWSTATION_@@_UPGRADE@@@__JY_VERSION__")													// 更新信号量
#define GWSTATION_APP_SEM							_T("{DA535439-57DB-4e03-9596-58DFC6C2BE9E}")													// 单实例内核对象

#define MH_LOGIN_UI_TIP									_T("")
#define MH_LOGIN_UI_TITLE								_T("金异黄金登录")
#define FRAMEWINDOW_TITLE								_T("金异黄金行情分析系统")
#define THANKS_USE_TIP									_T("再次感谢您对金异黄金的支持！")
#define REFFERAL_LINK_TIP								_T("推荐链接复制成功，请粘贴到您的QQ/微博上，推荐给您的好友！再次感谢您对金异黄金的支持！")
#define REFFERAL_CLIPBOARD_TIP							_T("金异黄金-您的掌上投资助手！行情分析、视频直播、投资策略应有尽有！即刻免费使用http://www.jinyi999.cn/%s")

#define ENABLE_ENTRUST_TRADE							// 开启实盘交易
#define ENABLE_SIMULATED_TRADE							// 模拟交易

#define ENABLE_AD										// 广告