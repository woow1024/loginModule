#include "StdAfx.h"
#include "GDILib.h"
#include <gwconst.h>
#include "FuncLib.h"

#ifdef SUPPORT_GDIPLUS
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;
#endif

const CString GDIOBJ_NAME[] = { _T("font"), _T("pen"), _T("brush") };
enum GDIOBJ_TYPE { GDI_FONT = 0, GDI_PEN, GDI_BRUSH };


namespace GW_UI
{
	CFont* GetChineseFont(int nSize, BOOL bBold)
	{
		return GDILib::GetInstance()->GetChineseFont(nSize, bBold);
	}

	CFont* GetNumberFont(int nSize, BOOL bBold)
	{
		return GDILib::GetInstance()->GetNumberFont(nSize, bBold);
	}

	CFont* GetEnglishFont(int nSize, BOOL bBold)
	{
		return GDILib::GetInstance()->GetEnglishFont(nSize,bBold);
	}

	COLORREF GetPriceColor(double dbPreClose, double dbPirce, int nDigit)
	{
#pragma GW_message(全局价格涨跌颜色函数获取待完善)
		double dbDiff =  dbPirce - dbPreClose;

		GWFloat vDiff(dbDiff, nDigit);

		if (vDiff.GetInnerValue() > 0)
		{
			return GW_PRICEUP_CLR;
		}
		else if (vDiff.GetInnerValue() < 0)
		{
			return GW_PRICEDOWN_CLR;
		}
		else
		{
			return GW_PRICEEQU_CLR;
		}
	}

	CPen* GetSplitLine(int nWidth, COLORREF clrLine, int nStyle)
	{
		return GetGDILib()->GetSplitLine(nWidth, clrLine, nStyle);
	}

	void LinePoint(CDC* pDC, CPoint pt1, CPoint pt2, CPen* pPen)
	{
		GDILib::LinePoint(pDC, pPen, pt1, pt2);
	}
}


GDILib* GetGDILib()
{
	return GDILib::GetInstance();
}

GDILib::~GDILib(void)
{
	for (GDIOBJMAPITR itr = m_mapGDIObj.begin(); itr != m_mapGDIObj.end(); ++itr)
	{
		itr->second->DeleteObject();
		delete itr->second;
	}

	m_mapGDIObj.clear();
}

CFont* GDILib::GetChineseFont(int nSize, BOOL bBold)
{
	return GetChineseFontEx(nSize, bBold);
}

CFont* GDILib::GetNumberFont(int nSize, BOOL bBold)
{
	return GetNumberFontEx(nSize, bBold);
}

CFont* GDILib::GetEnglishFont(int nSize, BOOL bBold)
{
	return GetEnglishFontEx(nSize, bBold);
}

CPen* GDILib::GetSplitLine(int nWidth, COLORREF clrLine, int nStyle)
{
	return GetPen(nWidth, clrLine, nStyle);
}

CFont* GDILib::GetChineseFontEx(int nSize, BOOL bBold, BOOL bUnderLine, const CString& strName)
{
	return CreateFont(
					nSize,										 // nHeight
					0,											 // nWidth
					0,											 // nEscapement
					0,											 // nOrientation
					bBold ? FW_BOLD : FW_NORMAL,			     // nWeight
					0,											 // bItalic
					bUnderLine,									 // bUnderline
					0,											 // cStrikeOut
					GB2312_CHARSET,								 // nCharSet 
					OUT_DEFAULT_PRECIS,							 // nOutPrecision
					CLIP_DEFAULT_PRECIS,						 // nClipPrecision
					DEFAULT_QUALITY,							 // nQuality
					DEFAULT_PITCH | FF_DONTCARE,				 // nPitchAndFamily
					strName										 // lpszFacename
					);
}

CFont* GDILib::GetNumberFontEx(int nSize, BOOL bBold, BOOL bUnderLine, const CString& strName)
{
	return CreateFont(
		nSize,										 // nHeight
		0,											 // nWidth
		0,											 // nEscapement
		0,											 // nOrientation
		bBold ? FW_BOLD : FW_NORMAL,			     // nWeight
		0,											 // bItalic
		bUnderLine,									 // bUnderline
		0,											 // cStrikeOut
		ANSI_CHARSET,								 // nCharSet 
		OUT_DEFAULT_PRECIS,							 // nOutPrecision
		CLIP_DEFAULT_PRECIS,						 // nClipPrecision
		DEFAULT_QUALITY,							 // nQuality
		DEFAULT_PITCH | FF_DONTCARE,				 // nPitchAndFamily
		strName										 // lpszFacename
		);
}

CFont* GDILib::GetEnglishFontEx(int nSize, BOOL bBold, BOOL bUnderLine, const CString& strName)
{
	return CreateFont(
		nSize,										 // nHeight
		0,											 // nWidth
		0,											 // nEscapement
		0,											 // nOrientation
		bBold ? FW_BOLD : FW_NORMAL,			     // nWeight
		0,											 // bItalic
		bUnderLine,									 // bUnderline
		0,											 // cStrikeOut
		ANSI_CHARSET,								 // nCharSet 
		OUT_DEFAULT_PRECIS,							 // nOutPrecision
		CLIP_DEFAULT_PRECIS,						 // nClipPrecision
		DEFAULT_QUALITY,							 // nQuality
		DEFAULT_PITCH | FF_DONTCARE,				 // nPitchAndFamily
		strName										 // lpszFacename
		);
}

CFont* GDILib::CreateFont(
						  int nHeight,               // height of font
						  int nWidth,                // average character width
						  int nEscapement,           // angle of escapement
						  int nOrientation,          // base-line orientation angle
						  int fnWeight,              // font weight
						  DWORD fdwItalic,           // italic attribute option
						  DWORD fdwUnderline,        // underline attribute option
						  DWORD fdwStrikeOut,        // strikeout attribute option
						  DWORD fdwCharSet,          // character set identifier
						  DWORD fdwOutputPrecision,  // output precision
						  DWORD fdwClipPrecision,    // clipping precision
						  DWORD fdwQuality,          // output quality
						  DWORD fdwPitchAndFamily,   // pitch and family
						  LPCTSTR lpszFace           // typeface name
						  )
{
	CString strKey;

	strKey.Format(_T("%d%d%d%d%d%d%d%d%d%d%d%d%d%s"),
					nHeight,
					nWidth,
					nEscapement,
					nOrientation,
					fnWeight,
					fdwItalic,
					fdwUnderline,
					fdwStrikeOut,
					fdwCharSet,
					fdwOutputPrecision,
					fdwClipPrecision,
					fdwQuality,
					fdwPitchAndFamily,
					lpszFace
				  );

	GDIOBJMAPITR itr = m_mapGDIObj.find(strKey);

	CFont *pFont = NULL;

	if (itr == m_mapGDIObj.end())
	{
		// 新建字体
		HFONT hFont= ::CreateFont(
			nHeight,
			nWidth,
			nEscapement,
			nOrientation,
			fnWeight,
			fdwItalic,
			fdwUnderline,
			fdwStrikeOut,
			fdwCharSet,
			fdwOutputPrecision,
			fdwClipPrecision,
			fdwQuality,
			fdwPitchAndFamily,
			lpszFace
			);

		ASSERT(hFont != NULL);
		
		if (hFont != NULL)
		{
			pFont = new CFont;
			pFont->Attach(hFont);

			m_mapGDIObj.insert(GDIOBJMAP::value_type(strKey, pFont));
		}
		
	}
	else
	{
		pFont = dynamic_cast<CFont*>(itr->second);
	}

	ASSERT(pFont != NULL);

	return pFont;
}

CPen* GDILib::GetPen(int nWidth, COLORREF crColor, int nPenStyle)
{
	CString strKey;

	strKey.Format(_T("%d%d%d%s"),
		nWidth,
		crColor,
		nPenStyle,
		GDIOBJ_NAME[GDI_PEN]
		);

	GDIOBJMAPITR itr = m_mapGDIObj.find(strKey);

	CPen *pPen = NULL;

	if (itr == m_mapGDIObj.end())
	{
		// 新建画笔
#if 1	//by w.s.h 能创建宽带大于1的 虚线画笔
		LOGBRUSH    logBrush;   
		ZeroMemory(&logBrush, sizeof(LOGBRUSH));
		logBrush.lbStyle    =    BS_SOLID;   
		logBrush.lbColor    =    crColor;   
		pPen = new CPen(nPenStyle|PS_GEOMETRIC,   nWidth,    &logBrush);
#else
		pPen = new CPen;
		pPen->CreatePen(nPenStyle, nWidth, crColor);
#endif
		m_mapGDIObj.insert(GDIOBJMAP::value_type(strKey, pPen));
	}
	else
	{
		pPen = dynamic_cast<CPen*>(itr->second);
	}

	ASSERT(pPen != NULL);

	return pPen;
}

void GDILib::LinePoint(CDC* pDC, CPen* pPen, int x1, int y1, int x2, int y2)
{
	CPen *pOldPen = pDC->SelectObject(pPen);
	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y2);
	pDC->SelectObject(pOldPen);
}

void GDILib::LinePoint(CDC* pDC, CPen* pPen, CPoint pt1, CPoint pt2)
{
	LinePoint(pDC, pPen, pt1.x, pt1.y, pt2.x, pt2.y);
}

void GDILib::DrawRectangle(CDC* pDC, CPen* pPen, const CRect& rect)
{
	LinePoint(pDC, pPen, rect.left, rect.top, rect.right, rect.top);
	LinePoint(pDC, pPen, rect.right, rect.top, rect.right, rect.bottom);
	LinePoint(pDC, pPen, rect.right, rect.bottom, rect.left, rect.bottom);
	LinePoint(pDC, pPen, rect.left, rect.bottom, rect.left, rect.top);
}

void GDILib::DrawRectangle(CDC* pDC, const CRect& rect, COLORREF clrPen, int nPenWidth, COLORREF clrBK, int nPenStyle)
{
	CBrush brush;
	brush.CreateSolidBrush(clrBK);

	CPen pen;
	pen.CreatePen(nPenStyle, nPenWidth, clrPen);

	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->Rectangle(rect);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

	brush.DeleteObject();
	pen.DeleteObject();
}

void GDILib::DrawRectangle(CDC* pDC, const CRect& rect, COLORREF clrPen, int nPenWidth, int nPenStyle)
{
	Color clr;
	clr.SetFromCOLORREF(clrPen);

	Pen pen(clr, (REAL)nPenWidth);
	pen.SetAlignment(PenAlignmentInset);

	Graphics graph(pDC->m_hDC);
	Rect rc(rect.left, rect.top, rect.Width(), rect.Height());
	pen.SetDashStyle((DashStyle)nPenStyle);
	graph.DrawRectangle(&pen, rc);
}

CBrush* GDILib::GetSolideBrush(COLORREF crColor)
{
	CString strKey;

	strKey.Format(_T("%d%s"),
		crColor,
		GDIOBJ_NAME[GDI_BRUSH]
	);

	GDIOBJMAPITR itr = m_mapGDIObj.find(strKey);

	CBrush *pBrush = NULL;

	if (itr == m_mapGDIObj.end())
	{
		// 新建字体
		pBrush = new CBrush;
		pBrush->CreateSolidBrush(crColor);
		m_mapGDIObj.insert(GDIOBJMAP::value_type(strKey, pBrush));
	}
	else
	{
		pBrush = dynamic_cast<CBrush*>(itr->second);
	}

	ASSERT(pBrush != NULL);

	return pBrush;
}

void GDILib::DrawRectText( CDC* pDC,CString strText,RECT rect,CFont* pFont,COLORREF clrText,UINT uFormat,E_TEXT_LAYOUT eLayout /*= E_TLAYOUT_HOR*/ )
{
	if (pDC == NULL) return;
	CFont* pOldFont = pDC->SelectObject(pFont);
	COLORREF clrOld = pDC->SetTextColor(clrText);
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	CRect rt(rect);
	if (eLayout == E_TLAYOUT_HOR)
	{
		pDC->DrawText(strText,rt,uFormat);
	}	
	else
	{
		//int nCount = strText.GetLength();
		//if (nCount != 0)
		//{
		//	CRect rtRealText;
		//	pDC->DrawText(strText,rtRealText,DT_SINGLELINE | DT_CALCRECT);
		//	long lTotalHigh = (nCount + 1) * BILLBOARD_VERTICAL_TEXT_GAP + rtRealText.Height() * nCount;
		//	long lWidth =  rtRealText.Width() / nCount;
		//	const TCHAR *p = strText.GetString();

		//	int y = rect.top;
		//	while (*p)
		//	{
		//		int nCount = (*p < 0) ? 2 : 1;

		//		int x = rect.left + ((rect.right - rect.left) - lWidth)/2;
		//		pDC->TextOut(x, y, p, nCount);
		//		p += nCount;
		//		y += rtRealText.Height() + BILLBOARD_VERTICAL_TEXT_GAP;
		//	}
		//}
	}

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(clrOld);
	pDC->SetBkMode(nOldMode);
}

void GDILib::FillPolygon( CDC* pDC,CPoint* pPoint,int nCount,COLORREF clr,BYTE byAlpha )
{
	if (pDC == NULL || pPoint == NULL || nCount < 0 ) return;
	if(pDC->GetSafeHdc() == NULL) return;
#ifdef SUPPORT_GDIPLUS
	Graphics graphics(pDC->m_hDC);
	Color color(byAlpha,GetRValue(clr),GetGValue(clr),GetBValue(clr));
	SolidBrush brush(color);

	Point* pt = new Point[nCount];
	for (int i = 0; i < nCount; i++)
	{
		pt[i].X = pPoint[i].x;
		pt[i].Y = pPoint[i].y;
	}

	graphics.FillPolygon(&brush,pt,nCount);
	delete []pt;
#else
	return;
#endif
}

void GDILib::DrawCurve( CDC* pDC,CPoint * pPoint,int nCount, COLORREF clr,int nWidth /*= 1*/,BYTE byAlpha /*= 0XFF*/ )
{
	if (pDC == NULL || pPoint == NULL || nCount <= 0) return;
	if(pDC->GetSafeHdc() == NULL) return;
#ifdef SUPPORT_GDIPLUS
	Graphics graphics(pDC->m_hDC);
	Pen pen(Color(byAlpha,GetRValue(clr),GetGValue(clr),GetBValue(clr)),REAL(nWidth));
	Point* pt = new Point[nCount];
	for (int i = 0; i < nCount; i++)
	{
		pt[i].X = pPoint[i].x;
		pt[i].Y = pPoint[i].y;
	}

	graphics.DrawLines(&pen,pt,nCount);
	delete []pt;
#else
	CPen pen(PS_SOLID,nWidth,clr);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->Polyline(pPoint,nCount);
	pDC->SelectObject(pOldPen);
#endif
}

#ifdef SUPPORT_GDIPLUS
BOOL GWBitmapEx::LoadFromFile(LPCTSTR lpFileName)
{
	Destory();

	if (!GW_PathFile::IsPathFileExist(lpFileName))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CStringW strFileName(lpFileName);

	m_pBmp = Bitmap::FromFile(strFileName);
	ASSERT(m_pBmp != NULL);

	if (m_pBmp == NULL)
	{
		return FALSE;
	}

	Status status = m_pBmp->GetLastStatus();
	ASSERT(status == Ok);

	if (status != Ok)
	{
		return FALSE;
	}

	InitImageFrameInfo();

	return TRUE;
}

BOOL GWBitmapEx::LoadFromRes(UINT uResID)
{
	Destory();
	if(NULL == uResID)
	{
		return FALSE;
	} 
	CBitmap  cBmp;
	BOOL bRet = cBmp.LoadBitmap(uResID);
	if(FALSE == bRet)
	{
		ASSERT(0);
		return FALSE;
	}
	HBITMAP hBMP = (HBITMAP)cBmp;
	m_pBmp = Bitmap::FromHBITMAP(hBMP, NULL);	
	if(NULL == m_pBmp)
	{
		ASSERT(0);
		return FALSE;
	}
	return TRUE;
}

GWBitmapEx::GWBitmapEx(LPCTSTR lpFileName) : c_GifGuid(FrameDimensionTime)
{
	m_pBmp			= NULL;
	m_nSleepTime	= 0;
	m_nFrameCount	= m_nActiveFrame = 0;
	m_bWorking		= FALSE;
	m_bLoop			= FALSE;
	m_hAttachWnd	= NULL;

	m_rcBmp.SetRectEmpty();

	if (lpFileName)
	{
		LoadFromFile(lpFileName);
	}
}

GWBitmapEx::~GWBitmapEx()
{
	Destory();
}

void GWBitmapEx::Destory()
{
	Stop();

	if (m_pBmp != NULL)
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}

	m_vecFrameDelay.clear();
	m_nFrameCount = m_nActiveFrame = 0;
	m_rcBmp.SetRectEmpty();
	m_bWorking = FALSE;
	m_bLoop = FALSE;
}

int GWBitmapEx::Width()
{
	if (m_pBmp == NULL)
	{
		return 0;
	}

	return m_pBmp->GetWidth();
}

int GWBitmapEx::Height()
{
	if (m_pBmp == NULL)
	{
		return 0;
	}

	return m_pBmp->GetHeight();
}

CSize GWBitmapEx::Size()
{
	return CSize(Width(), Height());
}

HBITMAP GWBitmapEx::GetHBitmapObj(COLORREF crTransparent)
{
	Gdiplus::Color color;
	color.SetFromCOLORREF(crTransparent);

	HBITMAP hBmp = NULL;
	Status status = m_pBmp->GetHBITMAP(color, &hBmp);

	ASSERT( status == Ok);

	return hBmp;
}

void GWBitmapEx::TransparentDraw(CDC* pDC, const CRect& rect, COLORREF crTransparent)
{
	TransparentDraw(pDC, rect, 0, 0, Width(), Height(), crTransparent);
}

void GWBitmapEx::TransparentDraw(CDC* pDC, const CRect& rect, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, COLORREF crTransparent)
{
	ASSERT(m_pBmp != NULL);

	if (m_pBmp == NULL)
	{
		return;
	}

	HBITMAP hBmp = GetHBitmapObj(crTransparent);

	if (hBmp == NULL)
	{
		return;
	}

	CBitmap* pBitmap = CBitmap::FromHandle(hBmp);
	ASSERT(pBitmap != NULL);

	CDC mDC;
	mDC.CreateCompatibleDC(pDC);
	mDC.SelectObject(pBitmap);

	::TransparentBlt(pDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(), mDC.GetSafeHdc(), xSrc, ySrc, nSrcWidth, nSrcHeight, crTransparent);

	mDC.SelectObject((CBitmap*)NULL);
	
	DeleteObject(hBmp);
	mDC.DeleteDC();
}

void GWBitmapEx::Draw(CDC* pDC, const CRect& rect)
{
	if (m_pBmp == NULL)
	{
		return ;
	}
	Graphics cs(pDC->GetSafeHdc());
	
	Status status = cs.DrawImage(m_pBmp, rect.left, rect.top, rect.Width(), rect.Height());
	ASSERT(status == Ok || status == ObjectBusy);
}

void GWBitmapEx::Draw(CDC* pDC, CRect& rect, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight)
{
	if (m_pBmp == NULL)
	{
		return ;
	}
	Graphics cs(pDC->GetSafeHdc());

	Rect rcDest(rect.left, rect.top, rect.Width(), rect.Height());

	Status status = cs.DrawImage(m_pBmp, rcDest, xSrc, ySrc, nSrcWidth, nSrcHeight, UnitPixel);
	ASSERT(status == Ok || status == ObjectBusy);
}

void GWBitmapEx::Draw(CDC* pDC)
{
	if (m_pBmp == NULL)
	{
		return ;
	}
	ASSERT(!m_rcBmp.IsRectEmpty());
	Draw(pDC, m_rcBmp);
}

void GWBitmapEx::WrapModeTileDraw(CDC* pDC, CRect& rect)
{
	Graphics cs(pDC->GetSafeHdc());

	TextureBrush brush(m_pBmp, WrapModeTile); 

	//Status status = cs.FillRectangle(&brush, RectF(0.0f, 0.0f, REAL(rect.right), REAL(rect.bottom)));
	Status status = cs.FillRectangle(&brush, RectF( REAL(rect.left), REAL(rect.top), REAL(rect.right), REAL(rect.bottom)));

	ASSERT(status == Ok || status == ObjectBusy);
}

void GWBitmapEx::Play(HWND hWnd, BOOL bLoop)
{
	if (m_bWorking || m_pBmp == NULL || hWnd == NULL)
	{
		return;
	}

	if (!IsMultiFrames())
	{
		return;
	}

	m_bLoop = bLoop;
	m_hAttachWnd = hWnd;
	m_bWorking = TRUE;

	if (m_rcBmp.IsRectEmpty())
	{
		m_rcBmp.SetRect(0, 0, m_pBmp->GetWidth(), m_pBmp->GetHeight());
	}

	SafeStartThread(&GWBitmapEx::PlayThread);
}

void GWBitmapEx::SetActiveFrame(UINT nFrame)
{
	ASSERT(nFrame >= 0 && nFrame < m_nFrameCount);

	if (nFrame < 0 || nFrame >= m_nFrameCount)
	{
		return;
	}

	m_nActiveFrame = nFrame;
}

BOOL GWBitmapEx::InitImageFrameInfo()
{
	GUID *guids = NULL;
	PropertyItem *pItem = NULL;

	m_vecFrameDelay.clear();
	m_nFrameCount = m_nActiveFrame = 0;

	__try
	{
		__try
		{
			UINT nDimensionsCount = m_pBmp->GetFrameDimensionsCount();	// 维度

			if (nDimensionsCount < 1 || m_pBmp->GetLastStatus() != Ok)
			{
				return FALSE;
			}

			guids = new GUID[nDimensionsCount];
			m_pBmp->GetFrameDimensionsList(guids, nDimensionsCount);

			if (m_pBmp->GetLastStatus() != Ok)
			{
				return FALSE;
			}

			m_nFrameCount = m_pBmp->GetFrameCount(&guids[0]);

			if (m_pBmp->GetLastStatus() != Ok)
			{
				return FALSE;
			}

			UINT uPISzie = m_pBmp->GetPropertyItemSize(PropertyTagFrameDelay);

			pItem = (PropertyItem *)malloc(uPISzie);
			m_pBmp->GetPropertyItem(PropertyTagFrameDelay, uPISzie, pItem);

			if (m_pBmp->GetLastStatus() != Ok)
			{
				return FALSE;
			}

			m_vecFrameDelay.reserve(m_nFrameCount);

			for (UINT n = 0; n < m_nFrameCount; ++n)
			{
				long lPause = ((long*)pItem->value)[n] * 10;
				m_vecFrameDelay.push_back(lPause);
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			return FALSE;
		}
	}
	__finally
	{
		if (guids != NULL)
		{
			delete guids;
			guids = NULL;
		}

		if (pItem != NULL)
		{
			free(pItem);
			pItem = NULL;
		}
	}

	return TRUE;
}

void GWBitmapEx::Move2Rect(const CRect& rect)
{
	m_rcBmp.CopyRect(rect);
}

void GWBitmapEx::Stop()
{
	if (!m_bWorking)
	{
		return;
	}

	m_bWorking = FALSE;

	JoinAllThread();
}

UINT GWBitmapEx::PlayThread()
{
	ASSERT( m_hAttachWnd != NULL);

	if (m_hAttachWnd == NULL)
	{
		return 0;
	}

	HDC hDC = GetDC(m_hAttachWnd);

	Graphics gh(hDC); //hDC是外部传入的画图DC

	try
	{
		while(m_bWorking)
		{
			m_pBmp->SelectActiveFrame(&c_GifGuid, m_nActiveFrame);

			//Draw(CDC::FromHandle(hDC));
			Draw(CDC::FromHandle(hDC), m_rcBmp);

			//重新设置当前的活动数据帧
			++m_nActiveFrame;

			if(m_nActiveFrame == m_nFrameCount) 
			{
				if (!m_bLoop)
				{
					m_bWorking = FALSE;
					break;
				}

				m_nActiveFrame = 0;  

				Sleep(m_nSleepTime);
				continue;
			}

			//计算此帧要延迟的时间
			if (!m_vecFrameDelay.empty())
			{
				long lPause = m_vecFrameDelay[m_nActiveFrame];
				
				lPause = min(lPause, 1000 * 60);
				m_nSleepTime = lPause;
				Sleep(lPause);       
			}
			else
			{
				m_bWorking = FALSE;
				break;
			}
		}
	}
	catch(...)
	{
		OutputDebugString(_T("GWBitmapEx::PlayThread error\n"));
	}

	ReleaseDC(m_hAttachWnd, hDC);

	return TRUE;
}
#else
BOOL GWBitmap::LoadFromFile(LPCTSTR lpFileName)
{
	Destroy();

	CFile file;

	if (!file.Open(lpFileName, CFile::modeRead | CFile::shareDenyNone))	
	{
		ASSERT(FALSE);
		return FALSE;
	}

	UINT dwSize = (UINT)file.GetLength();

	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	LPVOID lpBuf = ::GlobalLock(hMem);

	file.Read(lpBuf, dwSize);
	file.Close();

	::GlobalUnlock(hMem);

	IStream * pStream = NULL;
	IPicture * pPicture = NULL;

	// 由 HGLOBAL 得到 IStream，参数 TRUE 表示释放 IStream 的同时，释放内存
	HRESULT hr = ::CreateStreamOnHGlobal(hMem, TRUE, &pStream);

	if (FAILED(hr))
	{
		return FALSE;
	}

	hr = ::OleLoadPicture(pStream, dwSize, TRUE, IID_IPicture, (LPVOID *)&pPicture);

	if (FAILED(hr))
	{
		if (hr == E_POINTER )
		{
			return FALSE;
		}
		else if (hr == E_NOINTERFACE )
		{
			return FALSE;
		}
		return FALSE;
	}

	long nWidth, nHeight; // 宽高，MM_HIMETRIC 模式，单位是0.01毫米
	pPicture->get_Width(&nWidth); // 宽
	pPicture->get_Height(&nHeight); // 高

	// 将图像拷贝到自己
	CWindowDC dc(NULL);  

	CDC dcThis;
	dcThis.CreateCompatibleDC(&dc);  
	
	CSize sz(nWidth, nHeight);		
	dcThis.HIMETRICtoDP(&sz);

	CreateCompatibleBitmap(&dc, sz.cx, sz.cy); 

	CBitmap* pOldBitmap = dcThis.SelectObject(this);  
	pPicture->Render(dcThis.GetSafeHdc(), 0, 0, sz.cx, sz.cy, 0, nHeight, nWidth, -nHeight, NULL);
	dcThis.SelectObject(pOldBitmap);  

	dcThis.DeleteDC();
	dc.DeleteDC();

	if (pPicture)
	{
		pPicture->Release();// 释放 IPicture 指针
	}

	if (pStream)
	{
		pStream->Release(); // 释放 IStream 指针，同时释放了 hMem
	}

	return TRUE;
}

CSize GWBitmap::Size()
{
	BITMAP bm = {0};
	if (m_hObject != NULL)
	{
		GetBitmap(&bm);
	}

	return CSize(bm.bmWidth, bm.bmHeight);
}

void GWBitmap::TransparentBlt(CDC *pDC, CRect& rc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, COLORREF clrMask)
{
	CDC mDC;
	mDC.CreateCompatibleDC(pDC);
	CBitmap * pBmp = mDC.SelectObject(this);

	//pDC->TransparentBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mDC, xSrc, ySrc, nSrcWidth, nSrcHeight, clrMask);
	::TransparentBlt(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(), mDC.GetSafeHdc(), xSrc, ySrc, nSrcWidth, nSrcHeight, clrMask);

	mDC.SelectObject(pBmp);
	mDC.DeleteDC();
}

void GWBitmap::Draw(CDC* pDC, CRect& rect)
{
	CDC mDC;
	mDC.CreateCompatibleDC(pDC);
	CBitmap * pBmp = mDC.SelectObject(this);

	BITMAP bm;
	GetBitmap(&bm);

	pDC-> SetStretchBltMode(HALFTONE); 

	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &mDC, 0, 0, SRCCOPY);

	mDC.SelectObject(pBmp);
	mDC.DeleteDC();
}

void GWBitmap::Draw(CDC* pDC, CRect& rect, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight)
{
	CDC mDC;
	mDC.CreateCompatibleDC(pDC);
	CBitmap * pBmp = mDC.SelectObject(this);

	pDC-> SetStretchBltMode(HALFTONE); 
	pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &mDC, xSrc, ySrc, nSrcWidth, nSrcHeight, SRCCOPY);

	mDC.SelectObject(pBmp);
	mDC.DeleteDC();
}

void GWBitmap::Destroy()
{
	if (GetSafeHandle() != NULL)
	{
		DeleteObject();
	}
}

#endif
