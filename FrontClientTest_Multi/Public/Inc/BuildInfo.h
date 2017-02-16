#pragma 

#if defined(VERSION_TAG_TT)
#include "define_for_tt.h"
#elif defined(VERSION_TAG_TD)
#include "define_for_td.h"
#elif defined(VERSION_TAG_YG)
#include "define_for_yg.h"
#elif defined(VERSION_TAG_ZY)
#include "define_for_zy.h"
#elif defined(VERSION_TAG_XYYH)
#include "define_for_xyyh.h"
#elif defined(VERSION_TAG_QDZG)
#include "define_for_qdzg.h"
#elif defined(VERSION_TAG_BSY)
#include "define_for_bsy.h"
#elif defined(VERSION_TAG_SY)
#include "define_for_sy.h"
#elif defined(VERSION_TAG_DX)
#include "define_for_dx.h"
#elif defined(VERSION_TAG_JD)
#include "define_for_jd.h"
#elif defined(VERSION_TAG_JY)
#include "define_for_jy.h"
#else
#error 工程缺少版本定义(VERSION_TAG_XXXX)
#endif

#define INNER_BUILD_VERSION				2016031102
#define	INNER_RELEASE_DATE				"2016-03-11"

//#define VERSION_TAG_TEXT				""

//#define _GWSTATION_RELEASE_VERSION_BUILDING   "_GWSTATION_RELEASE_VERSION_BUILDING"