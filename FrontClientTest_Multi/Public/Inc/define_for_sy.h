#pragma once

/************************************************************************/
/*  2015-01-14 银天下石油版专用头文件                                     */
/************************************************************************/

#ifndef VERSION_TAG_SY
#error 引用了深石油版头文件却没有定义VERSION_TAG_SY宏，无法继续编译
#endif

#define VERSION_ACCOUNTTYPE								8	
#define VERSION_TAG										_T("SY")			
#define GW_VERSION_NAME									_T("深圳市前海银天下石油化工投资有限公司")

#define REMOTE_UPDATE_DIR								_T("/upgrade/sy/")																				// 升级下载目录
#define REMOTE_JIAOYI_DIR								_T("jiaoyi/sy")																					// 交易端下载目录

#define VERSION_SERVICE_PHONENUM						_T("400-1699-003")																					// 深石油版联系电话
#define VERSION_TAG_CODE								_T("SY")																							// 深石油版代码
#define GWSTATION_UPGRADE_SEM							_T("__@@GWSTATION_@@_UPGRADE@@@__SY_VERSION__")													// 更新信号量
#define GWSTATION_APP_SEM							_T("{03DAEFD6-2D08-4427-AEF7-B6C5763D0AF1}")													// 单实例内核对象

#define MH_LOGIN_UI_TIP									_T("")
#define MH_LOGIN_UI_TITLE								_T("银天下石油登录")
#define FRAMEWINDOW_TITLE								_T("银天下石油行情分析系统")
#define THANKS_USE_TIP									_T("再次感谢您对银天下石油的支持！")
#define REFFERAL_LINK_TIP								_T("推荐链接复制成功，请粘贴到您的QQ/微博上，推荐给您的好友！再次感谢您对银天下石油的支持！")
#define REFFERAL_CLIPBOARD_TIP							_T(" 石油投资用啥软件？必备利器——银天下！速度快，系统稳，功能强！即刻免费使用%s")

#define ENABLE_ENTRUST_TRADE							// 开启实盘交易
#define ENABLE_SIMULATED_TRADE							// 模拟交易

#define ENABLE_AD										// 广告