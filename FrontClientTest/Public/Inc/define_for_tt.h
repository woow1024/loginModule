#pragma once

/************************************************************************/
/*  2014-10-07 融汇版专用头文件                                         */
/************************************************************************/

#ifndef VERSION_TAG_TT
#error 引用了融汇版头文件却没有定义VERSION_TAG_TT宏，无法继续编译
#endif

#define VERSION_ACCOUNTTYPE								1							
#define VERSION_TAG										_T("tt")			
#define GW_VERSION_NAME									_T("天津融金汇银贵金属经营有限公司")	//

#define REMOTE_UPDATE_DIR								_T("/upgrade/tt/")			// 升级下载目录
#define REMOTE_JIAOYI_DIR								_T("jiaoyi/tt")					// 交易端下载目录

#define VERSION_SERVICE_PHONENUM						_T("021-95049")						// 融汇版联系电话
#define VERSION_TAG_CODE								_T("TT")													// 融汇版代码
#define GWSTATION_UPGRADE_SEM							_T("__@@GWSTATION_@@_UPGRADE@@@__TT_VERSION__")													// 更新信号量
#define GWSTATION_APP_SEM							_T("{E3B64050-7C05-41cc-A997-C91C42B7166A}")													// 单实例内核对象

#define MH_LOGIN_UI_TIP									_T("")//现货投资服务领导品牌
#define MH_LOGIN_UI_TITLE								_T("银天下登录")
#define FRAMEWINDOW_TITLE								_T("银天下行情分析系统")
#define THANKS_USE_TIP									_T("再次感谢您对银天下的支持！")
#define REFFERAL_LINK_TIP								_T("推荐链接复制成功，请粘贴到您的QQ/微博上，推荐给您的好友！再次感谢您对银天下的支持！")
#define REFFERAL_CLIPBOARD_TIP							_T(" 贵金属投资用啥软件？必备利器——银天下！速度快，系统稳，功能强！即刻免费使用%s")

#define ENABLE_ENTRUST_TRADE							// 开启实盘交易
#define ENABLE_SIMULATED_TRADE							// 模拟交易
#define ENABLE_AD										// 广告