#pragma once

#include "StdAfx.h"
#include <gwtypes.h>
#include "Markup.h"
#include "GlobalSetting.h"
#include "FuncLib.h"
#include <algorithm>
using namespace std;

namespace GW_Cfg
{
	GW_DLLEXPORT GWVOID ReadIndicatorXML(CIndicatorArr & CTAArr, const CString & strKey)
	{
		CTAArr.RemoveAll();
		CString strCfgXML =GetGloablSetting()->GetGFSubDir(GDT_TACFG) + _T("Indicator.xml");
		
		if(FALSE == GW_PathFile::IsPathFileExist(strCfgXML))
		{
			ASSERT(0);
			return ;
		}
		CMarkup cXMLParser;
		BOOL bRet =cXMLParser.Load((LPCTSTR)strCfgXML);	
		if(FALSE == bRet)
		{
			return ;
		}
		cXMLParser.ResetMainPos();
		bRet = cXMLParser.FindElem(_T("root"));
		if(FALSE == bRet)//父节点
		{
			return ;
		}
		cXMLParser.IntoElem();
		bRet = cXMLParser.FindElem((LPCTSTR)strKey);
		if(FALSE == bRet)//父节点
		{
			return ;
		}
		cXMLParser.IntoElem();	
		while(cXMLParser.FindElem(_T("ITEM")))//加载的模块节点
		{
			CString strName = cXMLParser.GetAttrib(_T("name"));
			CString strAlias = cXMLParser.GetAttrib(_T("Alias"));
			tagIndicatorItem tCfgIndicator;
			tCfgIndicator.strName = strName;
			tCfgIndicator.strAlias = strAlias;
			cXMLParser.IntoElem();
			if(cXMLParser.FindElem(_T("ChineseName")))
			{
				tCfgIndicator.strChineseName = cXMLParser.GetData();
			}
			if(cXMLParser.FindElem(_T("Group")))
			{
				CString strID = cXMLParser.GetData();
				DWORD dwID = _ttol((LPCTSTR)strID);
				tCfgIndicator.byIndex = (BYTE)dwID;
			}
			int nLineNum = 0;
			while(cXMLParser.FindElem(_T("Descrption")))
			{
				CString strLine = cXMLParser.GetData();
				strLine += "   ";
				tCfgIndicator.strDescptionArr[nLineNum] = strLine;
				nLineNum ++;
				if(nLineNum >= 10)
				{
					break;
				}
			}
			tCfgIndicator.nDescptionLineNum = nLineNum;
			cXMLParser.OutOfElem();

			CTAArr.Add(tCfgIndicator);
		}
		cXMLParser.OutOfElem();

		cXMLParser.OutOfElem();	
	}

	GW_DLLEXPORT CString GetUrlFromCfg(LPCTSTR lpApp, LPCTSTR lpSection)
	{
		CString strUrl = GetRegKeyString(GlobalSetting::GetInstance()->GetGFSubDir(GDT_CFG) + _T("urlconfig.ini"), lpApp, lpSection);

		return strUrl;
	}

	GW_DLLEXPORT CString GetRegKeyString(const CString& strFile, const CString& strSection, const CString& strKey, const CString& strDefault)
	{
		CString strValue;
		const int nBufferLen = 1024 * 4;

		::GetPrivateProfileString(strSection, strKey, strDefault, strValue.GetBuffer(nBufferLen), nBufferLen, strFile);
		strValue.ReleaseBuffer(-1);
		return strValue;
	}

	GW_DLLEXPORT int GetRegKeyInt(const CString& strFile, const CString& strSection, const CString& strKey, int lValDefalt)
	{
		int nVal = ::GetPrivateProfileInt(strSection, strKey, lValDefalt, strFile);
		return nVal;
	}

	GW_DLLEXPORT GWBOOL WriteRegKeyString( const CString& strFile,  const CString& strSection,  const CString& strKey, const CString& strVal)
	{
		return ::WritePrivateProfileString(strSection, strKey, strVal, strFile);
	}

	GW_DLLEXPORT GWBOOL WriteRegKeyInt(const CString& strFile, const CString& strSection, const CString& strKey, int lVal)
	{
		CString strVal;
		strVal.Format(_T("%d"), lVal);

		return ::WritePrivateProfileString(strSection, strKey, strVal, strFile);
	}
}


namespace GW_InfoBomb
{
	GW_FUNCLIB_API 	void ExtractDelInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecDel)
	{
		vecDel.clear();

		int nBombNum = vecOrg.size();
		GWINFOBOMVECITR cIter = vecOrg.begin();
		for(;cIter != vecOrg.end(); )
		{
			if(GWInfoBom::em_Bomb_Del == cIter->nState)
			{
				vecDel.push_back(*cIter);
				cIter = vecOrg.erase(cIter);
			}
			else
			{
				cIter ++;
			}
		} 
	}
	GW_FUNCLIB_API 	void RemoveUnusedInfoBomb(IN OUT GWINFOBOMVEC& vecOrg, IN const GWINFOBOMVEC & vecDel)
	{
		int nDelNum = vecDel.size();
		for(int ii = 0; ii < nDelNum; ii ++ )
		{
			const GWInfoBom & tInfoBombDel = vecDel[ii];
			int nBombNum = vecOrg.size();
			GWINFOBOMVECITR cIter = vecOrg.begin();
			for(;cIter != vecOrg.end(); )
			{
				if( tInfoBombDel.dwArticalID == cIter->dwArticalID)
				{//栏目ID 一致 且 文章 ID一致，表示同一个
					cIter = vecOrg.erase(cIter);
				}
				else
				{
					cIter ++;
				}
			} 
		}

	}


	bool InfoBomb_Equal(GWInfoBom& tB1, GWInfoBom& tB2)
	{
		if(tB1.dwArticalID == tB2.dwArticalID)
		{//栏目ID 一致 且 文章 ID一致，表示同一个
			return TRUE;
		}
		return FALSE;
	};

	GW_FUNCLIB_API 	void SortAndUniqueInfoBombArr(IN OUT GWINFOBOMVEC& vecOrg, BOOL bKMode)
	{
		// 这里去除杂质 
		//if(bKMode)
		{
			//K线需要排序
			//sort(vecOrg.begin(), vecOrg.end(), GWObjCompareEx());

			sort(vecOrg.begin(), vecOrg.end(), GWObjCompare());
		} 

		//分时可以根据时间轴来确定索引，所以不需要排序
		vecOrg.erase(unique(vecOrg.begin(), vecOrg.end(), InfoBomb_Equal), vecOrg.end()); 
	}
	
	GW_FUNCLIB_API  void CheckUpdateInfoBombArr(IN OUT GWINFOBOMVEC& vecOrg)
	{
		int nBombNum = vecOrg.size();
		if(0 == nBombNum)
		{
			return;
		}
		GWINFOBOMVECITR cIter = vecOrg.begin();
		GWINFOBOMVECITR cIteEnd =vecOrg.end();
		for(; cIter != cIteEnd; cIter ++)
		{
			if( GWInfoBom::em_Bomb_Add== cIter->nState || GWInfoBom::em_Bomb_Del== cIter->nState)
			{

			}
			else
			{
				if(cIter->dwCreateTime != cIter->dwUpdateDateTime)
				{
					cIter->nState = GWInfoBom::em_Bomb_Update;
				}
			}
		}
	}

	GW_FUNCLIB_API 	void ExtractUpdateInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecUpdate)
	{
		vecUpdate.clear();
		GW_InfoBomb::CheckUpdateInfoBombArr(vecOrg);
		int nBombNum = vecOrg.size();
		if(0 == nBombNum)
		{
			return;
		}
		GWINFOBOMVECITR cIter = vecOrg.begin();
		for(;cIter != vecOrg.end(); )
		{
			if(GWInfoBom::em_Bomb_Update == cIter->nState)
			{
				vecUpdate.push_back(*cIter);
			} 
			cIter ++;			 
		} 

	}

	GW_FUNCLIB_API 	void MergeUpdateInfoBomb(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecUpdate)
	{
		int nDelNum = vecUpdate.size();
		for(int ii = 0; ii < nDelNum; ii ++ )
		{
			const GWInfoBom & tInfoBombDel = vecUpdate[ii];
			if(GWInfoBom::em_Bomb_Update != tInfoBombDel.nState)
			{
				ASSERT(0);
			}
			int nBombNum = vecOrg.size();
			if(0 == nBombNum)
			{
				return;
			}
			GWINFOBOMVECITR cIter = vecOrg.begin();
			for(;cIter != vecOrg.end(); )
			{
				if(tInfoBombDel.dwArticalID == cIter->dwArticalID)
				{//栏目ID 一致 且 文章 ID一致，表示同一个
					if(tInfoBombDel.dwUpdateDateTime > cIter->dwUpdateDateTime )
					{//如果时间小于当前更新的，则需要移除
						cIter = vecOrg.erase(cIter);
					}
					else
					{
						cIter ++;
					}
				}
				else
				{
					cIter ++;
				}
			} 
		}
	}

	GW_FUNCLIB_API  void RemovInfoBombByType(IN OUT GWINFOBOMVEC& vecOrg, IN int nType)
	{ 
		int nBombNum = vecOrg.size();
		GWINFOBOMVECITR cIter = vecOrg.begin();
		for(;cIter != vecOrg.end(); )
		{
			if(nType & cIter->nType)
			{ 
				cIter = vecOrg.erase(cIter);
			}
			else
			{
				cIter ++;
			} 
		}
	}


	GW_FUNCLIB_API 	void MergeTwoInfoBombArr(IN GWINFOBOMVEC& vecOrg, IN OUT GWINFOBOMVEC & vecNew, BOOL bNeedSortAndUnique, BOOL bKMode)
	{
		GWINFOBOMVEC vecDel;
		GWINFOBOMVEC vecUpdate;

		//提取需要删除的地雷队列
		GW_InfoBomb::ExtractDelInfoBombArr(vecNew, vecDel);

		GW_InfoBomb::RemoveUnusedInfoBomb(vecOrg, vecDel);


		//提取更新的队列
		GW_InfoBomb::ExtractUpdateInfoBombArr(vecNew, vecUpdate);
		//合并自身的更新队列
		GW_InfoBomb::MergeUpdateInfoBomb(vecNew, vecUpdate);
		
		GW_InfoBomb::MergeUpdateInfoBomb(vecOrg, vecUpdate);
		
		VECTOR_MERGE(vecOrg, vecNew, FALSE);
		if(bNeedSortAndUnique)
		{
			GW_InfoBomb::SortAndUniqueInfoBombArr(vecOrg, bKMode);
		}
	}

	GW_FUNCLIB_API 	void TrimeInfoBombArr(IN GWINFOBOMVEC& vecOrg,  BOOL bNeedSortAndUnique, BOOL bKMode)
	{		
		GWINFOBOMVEC vecDel;
		GWINFOBOMVEC vecUpdate;

		//提取需要删除的地雷队列
		GW_InfoBomb::ExtractDelInfoBombArr(vecOrg, vecDel);
		GW_InfoBomb::RemoveUnusedInfoBomb(vecOrg, vecDel);

		//提取更新的队列
		GW_InfoBomb::ExtractUpdateInfoBombArr(vecOrg, vecUpdate);
		//合并自身的更新队列
		GW_InfoBomb::MergeUpdateInfoBomb(vecOrg, vecUpdate);

		if(bNeedSortAndUnique)
		{
			GW_InfoBomb::SortAndUniqueInfoBombArr(vecOrg, bKMode);
		}
	}

	GW_FUNCLIB_API CString CreateInfoBomLink(const SymbolObj * pSymbolObj, const GWInfoBom & tGWInfoBomb, const CString& strFixLink)
	{
		// http://test.jry.baidao.com/mine/alert.do?serverId=1&tradeDate=2013-11-14&marketCode=TPME.XAGUSD&mineId=1&mineType=1
		CString strLink;
		CString strObj;
		CString strMarket;
		CString strMarketCode;
		if (pSymbolObj != NULL)
		{
			strObj = pSymbolObj->strCode;
			strMarket = pSymbolObj->pMarketIfo->strMarketName;
			strMarketCode.Format(_T("%s.%s"),(LPCTSTR)strMarket, (LPCTSTR)strObj);
		}
		else 
		{ 
			strMarketCode = _T("ALL");
		}
		CTime st(tGWInfoBomb.dwTradeDate);
		/*CString strDate = st.Format(_T("%Y-%m-%d"));
		strLink.Format("%sserverId=%d&tradeDate=%s&marketCode=%s&mineId=%d&mineType=%d",
			(LPCTSTR)strFixLink, VERSION_ACCOUNTTYPE, (LPCTSTR)strDate, (LPCTSTR)strMarketCode, tGWInfoBomb.dwArticalID, tGWInfoBomb.nType);*/
		strLink.Format("%s?type[equal]2",(LPCTSTR)strFixLink);
		return strLink;
	}

}


namespace GW_BigNews
{
	GW_FUNCLIB_API 	void ExtractDelNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecDel)
	{
		vecDel.clear();

		int nBombNum = vecOrg.size();
		GWBIGNEWSVECITR cIter = vecOrg.begin();
		for(;cIter != vecOrg.end(); )
		{
			if(GWBigNews::em_News_Del == cIter->nState)
			{
				vecDel.push_back(*cIter);
				cIter = vecOrg.erase(cIter);
			}
			else
			{
				cIter ++;
			}
		} 

	}

	GW_FUNCLIB_API 	void RemoveUnusedNews(IN OUT GWBIGNEWSVEC& vecOrg, IN const GWBIGNEWSVEC & vecDel)
	{
		int nDelNum = vecDel.size();
		for(int ii = 0; ii < nDelNum; ii ++ )
		{
			const GWBigNews & tDelItem = vecDel[ii];
			int nItemNum = vecOrg.size();
			GWBIGNEWSVECITR cIter = vecOrg.begin();
			for(;cIter != vecOrg.end(); )
			{
				if( tDelItem.dwNewsID == cIter->dwNewsID)
				{//栏目ID 一致 且 文章 ID一致，表示同一个
					cIter = vecOrg.erase(cIter);
				}
				else
				{
					cIter ++;
				}
			} 
		}

	}

	bool News_Equal(GWBigNews& tB1, GWBigNews& tB2)
	{
		if(tB1.dwNewsID == tB2.dwNewsID)
		{//栏目ID 一致 且 文章 ID一致，表示同一个
			return TRUE;
		}
		return FALSE;
	};

	GW_FUNCLIB_API 	void SortAndUniqueNewsArr(IN OUT GWBIGNEWSVEC& vecOrg )
	{
		// 这里去除杂质 
		sort(vecOrg.begin(), vecOrg.end(), GWObjCompare());
		//分时可以根据时间轴来确定索引，所以不需要排序
		vecOrg.erase(unique(vecOrg.begin(), vecOrg.end(), News_Equal), vecOrg.end()); 
	}
 

	GW_FUNCLIB_API 	void ExtractUpdateNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecUpdate)
	{
		vecUpdate.clear(); 
		int nItemNum = vecOrg.size();
		if(0 == nItemNum)
		{
			return;
		}
		GWBIGNEWSVECITR cIter = vecOrg.begin();
		for(;cIter != vecOrg.end(); )
		{
			if(GWBigNews::em_News_Update == cIter->nState)
			{
				vecUpdate.push_back(*cIter);
			} 
			cIter ++;			 
		} 

	}

	GW_FUNCLIB_API 	void MergeUpdateNews(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecUpdate)
	{
		int nDelNum = vecUpdate.size();
		for(int ii = 0; ii < nDelNum; ii ++ )
		{
			const GWBigNews & tNewsDel = vecUpdate[ii];
			if(GWBigNews::em_News_Update != tNewsDel.nState)
			{
				ASSERT(0);
			}
			int nItemNum = vecOrg.size();
			if(0 == nItemNum)
			{
				return;
			}
			GWBIGNEWSVECITR cIter = vecOrg.begin();
			for(;cIter != vecOrg.end(); )
			{
				if(tNewsDel.dwNewsID == cIter->dwNewsID)
				{//栏目ID 一致 且 文章 ID一致，表示同一个
					if(tNewsDel.dwUpdateDateTime > cIter->dwUpdateDateTime )
					{//如果时间小于当前更新的，则需要移除
						cIter = vecOrg.erase(cIter);
					}
					else
					{
						cIter ++;
					}
				}
				else
				{
					cIter ++;
				}
			} 
		}
	}

	GW_FUNCLIB_API  void RemovNewsByType(IN OUT GWBIGNEWSVEC& vecOrg, IN int nType)
	{ 
		int nItemNum = vecOrg.size();
		GWBIGNEWSVECITR cIter = vecOrg.begin();
		for(;cIter != vecOrg.end(); )
		{
			if(nType == cIter->nType)
			{ 
				cIter = vecOrg.erase(cIter);
			}
			else
			{
				cIter ++;
			} 
		}
	}


	GW_FUNCLIB_API 	void MergeTwoNewsArr(IN GWBIGNEWSVEC& vecOrg, IN OUT GWBIGNEWSVEC & vecNew, BOOL bNeedSortAndUnique)
	{
		GWBIGNEWSVEC vecDel;
		GWBIGNEWSVEC vecUpdate;

		//提取需要删除的地雷队列
		GW_BigNews::ExtractDelNewsArr(vecNew, vecDel);

		GW_BigNews::RemoveUnusedNews(vecOrg, vecDel);


		//提取更新的队列
		GW_BigNews::ExtractUpdateNewsArr(vecNew, vecUpdate);
		//合并自身的更新队列
		GW_BigNews::MergeUpdateNews(vecNew, vecUpdate);

		GW_BigNews::MergeUpdateNews(vecOrg, vecUpdate);

		VECTOR_MERGE(vecOrg, vecNew, FALSE);
		if(bNeedSortAndUnique)
		{
			GW_BigNews::SortAndUniqueNewsArr(vecOrg);
		}
	}
	

	GW_FUNCLIB_API 	void TrimeNewsArr(IN GWBIGNEWSVEC& vecOrg,  BOOL bNeedSortAndUnique)
	{
	
		GWBIGNEWSVEC vecDel;
		GWBIGNEWSVEC vecUpdate;

		//提取需要删除的地雷队列
		GW_BigNews::ExtractDelNewsArr(vecOrg, vecDel);
		GW_BigNews::RemoveUnusedNews(vecOrg, vecDel);

		//提取更新的队列
		GW_BigNews::ExtractUpdateNewsArr(vecOrg, vecUpdate);
		//合并自身的更新队列
		GW_BigNews::MergeUpdateNews(vecOrg, vecUpdate);

		if(bNeedSortAndUnique)
		{
			GW_BigNews::SortAndUniqueNewsArr(vecOrg);
		}
	}


	GW_FUNCLIB_API  int  GetValidNewsNum(IN GWBIGNEWSVEC& vecOrg)
	{
		int nValidNumCount = 0;
		GWBIGNEWSVECITR cIter = vecOrg.begin();
		for(;cIter != vecOrg.end(); cIter ++)
		{
			if(cIter->nState != GWBigNews::em_News_Del)
			{
				nValidNumCount ++;
			}
		}
		return nValidNumCount;
	}

	GW_FUNCLIB_API  void SaveNewsArr(IN GWBIGNEWSVEC& vecOrg, int  nMaxValidNum)
	{
		int nItemCount = vecOrg.size();
		if(nItemCount <= nMaxValidNum)
		{
			return;
		}
		GWBIGNEWSVECITR cIterB = vecOrg.begin();
		GWBIGNEWSVECITR cIterE = vecOrg.end();
		GWBIGNEWSVECITR cIter = --cIterE ;
		int nValidNumCount = 0; 
		for( ;cIter != cIterB; cIter --)
		{
			if(cIter->nState != GWBigNews::em_News_Del)
			{
				nValidNumCount ++;
			}
			if(nValidNumCount >= nMaxValidNum)
			{
				break;
			}
		}
		if(nValidNumCount < nMaxValidNum)
		{
			return;
		}
		vecOrg.erase(cIterB, cIter);
	}

	GW_FUNCLIB_API  void MarkUpdateNewsByTime(IN OUT GWBIGNEWSVEC& vecOrg, time_t dwLastTime, BOOL bForceRecover)
	{
		int nItemCount = vecOrg.size(); 
		for(int ii = 0; ii < nItemCount; ii ++)
		{
			if(bForceRecover)
			{
				vecOrg[ii].dwUserFlag =  tagBigNews::em_News_Normal;
			}
			else
			{
				if(vecOrg[ii].dwUpdateDateTime > dwLastTime)
				{
					vecOrg[ii].dwUserFlag = tagBigNews::em_News_MarkRefresh;
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CStringCoder::CStringCoder(void)  
{  
}  

CStringCoder::~CStringCoder(void)  
{  
}  
void CStringCoder::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)  
{  
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);  
	return;  
}  
void CStringCoder::UTF_8ToUnicode(WCHAR* pOut,char *pText)  
{  
	char* uchar = (char *)pOut;  

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);  
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);  

	return;  
}  

void CStringCoder::UnicodeToUTF_8(char* pOut,WCHAR* pText)  
{  
	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后  
	char* pchar = (char *)pText;  

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));  
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);  
	pOut[2] = (0x80 | (pchar[0] & 0x3F));  

	return;  
}  
void CStringCoder::UnicodeToGB2312(char* pOut,WCHAR uData)  
{  
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);  
	return;  
}  

//做为解Url使用  
char CStringCoder:: CharToInt(char ch){  
	if(ch>='0' && ch<='9')return (char)(ch-'0');  
	if(ch>='a' && ch<='f')return (char)(ch-'a'+10);  
	if(ch>='A' && ch<='F')return (char)(ch-'A'+10);  
	return -1;  
}  
char CStringCoder::StrToBin(char *str){  
	char tempWord[2];  
	char chn;  

	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011  
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0  -- 00000000  

	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000  

	return chn;  
}  


//UTF_8 转gb2312  
void CStringCoder::UTF_8ToGB2312(CString &pOut, char *pText, int pLen)  
{  
	char buf[4];  
	char* rst = new char[pLen + (pLen >> 2) + 2];  
	memset(buf,0,4);  
	memset(rst,0,pLen + (pLen >> 2) + 2);  

	int i =0;  
	int j = 0;  

	while(i < pLen)  
	{  
		if(*(pText + i) >= 0)  
		{  

			rst[j++] = pText[i++];  
		}  
		else                   
		{  
			WCHAR Wtemp;  


			UTF_8ToUnicode(&Wtemp,pText + i);  

			UnicodeToGB2312(buf,Wtemp);  

			unsigned short int tmp = 0;  
			tmp = rst[j] = buf[0];  
			tmp = rst[j+1] = buf[1];  
			tmp = rst[j+2] = buf[2];  

			//newBuf[j] = Ctemp[0];  
			//newBuf[j + 1] = Ctemp[1];  

			i += 3;      
			j += 2;     
		}  

	}  
	rst[j]='\0';  
	pOut = rst;   
	delete []rst;  
}  

//GB2312 转为 UTF-8  
void CStringCoder::GB2312ToUTF_8(CString& strOut,char *pText, int pLen)  
{  
	char buf[4];  
	memset(buf,0,4);  
	string pOut; 

	int i = 0;  
	while(i < pLen)  
	{  
		//如果是英文直接复制就可以  
		if( pText[i] >= 0)  
		{  
			char asciistr[2]={0};  
			asciistr[0] = (pText[i++]);  
			pOut.append(asciistr);  
		}  
		else  
		{  
			WCHAR pbuffer;  
			Gb2312ToUnicode(&pbuffer,pText+i);  

			UnicodeToUTF_8(buf,&pbuffer);  

			pOut.append(buf);  

			i += 2;  
		}  
	}  
	strOut = pOut.c_str();
	return;  
}  
//把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节  如%3D%AE%88  
CString CStringCoder::UrlGB2312(char * str)  
{  
	string dd;  
	size_t len = strlen(str);  
	for (size_t i=0;i<len;i++)  
	{  
		if(isalnum((BYTE)str[i]))  
		{  
			char tempbuff[2];  
			sprintf(tempbuff,"%c",str[i]);  
			dd.append(tempbuff);  
		}  
		else if (isspace((BYTE)str[i]))  
		{  
			dd.append("+");  
		}  
		else  
		{  
			char tempbuff[4];  
			sprintf(tempbuff,"%%%X%X",((BYTE*)str)[i] >>4,((BYTE*)str)[i] %16);  
			dd.append(tempbuff);  
		}  

	}  
	CString strOut;
	strOut = dd.c_str();
	return strOut;  
}  

//把str编码为网页中的 UTF-8 url encode ,英文不变，汉字三字节  如%3D%AE%88  

CString CStringCoder::UrlUTF8(char * str)  
{  
	CString strTemp;  
	string dd;  
	GB2312ToUTF_8(strTemp,str,(int)strlen(str));  
	string tt = (LPCTSTR)strTemp;
	size_t len=tt.length();  
	for (size_t i=0;i<len;i++)  
	{  
		if(isalnum((BYTE)tt.at(i)))  
		{  
			char tempbuff[2]={0};  
			sprintf(tempbuff,"%c",(BYTE)tt.at(i));  
			dd.append(tempbuff);  
		}  
		else if (isspace((BYTE)tt.at(i)))  
		{  
			dd.append("+");  
		}  
		else  
		{  
			char tempbuff[4];  
			sprintf(tempbuff,"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);  
			dd.append(tempbuff);  
		}  

	}  
	CString strOut = dd.c_str();
	return strOut;  
}  
//把url GB2312解码  
CString CStringCoder::UrlGB2312Decode(const CString& strURL)  
{  
	string output="";  
	char tmp[2];  
	string str = (LPCTSTR)strURL;
	int i=0,idx=0,len=str.length();  

	while(i<len){  
		if(str[i]=='%'){  
			tmp[0]=str[i+1];  
			tmp[1]=str[i+2];  
			output += StrToBin(tmp);  
			i=i+3;  
		}  
		else if(str[i]=='+'){  
			output+=' ';  
			i++;  
		}  
		else{  
			output+=str[i];  
			i++;  
		}  
	}  
	CString strOut = output.c_str();
	return strOut;  
}  
//把url utf8解码  
CString CStringCoder::UrlUTF8Decode(const  CString& str)  
{    
	CString strOut;
	CString strTmp =UrlGB2312Decode(str);//  
	string temp = (LPCTSTR)strTmp;
	UTF_8ToGB2312(strOut,(char *)temp.data(),strlen(temp.data()));  
 
	return strOut;  

}  