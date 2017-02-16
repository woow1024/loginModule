#pragma once

/************************************************************************/
/*  2014-10-07 银瑞版专用头文件                                         */
/************************************************************************/

#ifndef VERSION_TAG_YG
#error 引用了银瑞版头文件却没有定义VERSION_TAG_YG宏，无法继续编译
#endif

#define VERSION_ACCOUNTTYPE								3	
#define VERSION_TAG										_T("yg")			
#define GW_VERSION_NAME									_T("银如意行情分析系统")//_T("大象贵金属行情分析系统")//_T("广东金祥银瑞贵金属经营有限公司")

#define REMOTE_UPDATE_DIR								_T("/upgrade/yg/")																				// 升级下载目录
#define REMOTE_JIAOYI_DIR								_T("jiaoyi/yg")																					// 交易端下载目录

#define VERSION_SERVICE_PHONENUM						_T("400-1616-003")																				// 银瑞版联系电话
#define VERSION_TAG_CODE								_T("YG")																						// 银瑞版代码
#define GWSTATION_UPGRADE_SEM							_T("__@@GWSTATION_@@_UPGRADE@@@__YG_VERSION__")													// 更新信号量
#define GWSTATION_APP_SEM							_T("{1BA5937E-E17F-4eb9-8E48-6F756CFA744A}")													// 单实例内核对象

#define MH_LOGIN_UI_TIP									_T("")
#define MH_LOGIN_UI_TITLE								_T("银如意贵金属登录")
#define FRAMEWINDOW_TITLE								_T("银如意贵金属行情分析系统")
#define THANKS_USE_TIP									_T("再次感谢您对银如意贵金属的支持！")
#define REFFERAL_LINK_TIP								_T("推荐链接复制成功，请粘贴到您的QQ/微博上，推荐给您的好友！再次感谢您对银如贵金属意的支持！")
#define REFFERAL_CLIPBOARD_TIP							_T(" 贵金属投资用啥软件？必备利器——银如意！速度快，系统稳，功能强！即刻免费使用%s")


#define ENABLE_ENTRUST_TRADE							// 开启实盘交易
#define ENABLE_SIMULATED_TRADE							// 模拟交易
#define ENABLE_AD										// 广告