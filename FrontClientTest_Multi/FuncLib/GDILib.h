#pragma once

#include <toolclass.h>
#include <SafeThread.h>
#include <gwtypes.h>

#define SUPPORT_GDIPLUS

#ifdef SUPPORT_GDIPLUS
#include <GdiPlus.h>
#endif

#undef GW_EXT_CLASS
#ifdef GWFUNCLIB_PROJECT
#define GW_EXT_CLASS _declspec(dllexport) 
#else
#define GW_EXT_CLASS _declspec(dllimport)
#endif

class GW_EXT_CLASS GDILib : public static_singletonbase<GDILib>
{
	IMPLEMENT_STATIC_SINGLETON_MODE(GDILib)

	DECLARE_MAP(CString, CGdiObject*, GDIOBJ)
public:
	enum E_TEXT_LAYOUT
	{
		E_TLAYOUT_HOR = 0,			// 水平
		E_TLAYOUT_VER=1,				// 垂直
	};
public:
	~GDILib(void);
	
	CFont* GetChineseFont(int nSize, BOOL bBold = FALSE);					// 获取中文字体
	CFont* GetNumberFont(int nSize, BOOL bBold = FALSE);					// 获取数字字体
	CFont* GetEnglishFont(int nSize, BOOL bBold = FALSE);					// 获取英文字体
	
	CPen*  GetSplitLine(int nWidth, COLORREF clrLine, int nStyle);

	CFont* GetChineseFontEx(int nSize, BOOL bBold = FALSE, BOOL bUnderLine = FALSE, const CString& strName = _T("宋体"));		// 获取中文字体
	CFont* GetNumberFontEx(int nSize, BOOL bBold = FALSE, BOOL bUnderLine = FALSE, const CString& strName = _T("System"));		// 获取数字字体
	CFont* GetEnglishFontEx(int nSize, BOOL bBold = FALSE, BOOL bUnderLine = FALSE, const CString& strName = _T("Arial"));		// 获取英文字体

	CFont* CreateFont(																		// 原始字体创建，注意不要销毁指针
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
		);

	CPen* GetPen(int nWidth, COLORREF crColor, int nPenStyle = PS_SOLID);				// 创建Pen

	CBrush* GetSolideBrush(COLORREF crColor);											// 创建Brush
public:
	static void LinePoint(CDC* pDC, CPen* pPen, CPoint pt1, CPoint pt2);
	static void LinePoint(CDC* pDC, CPen* pPen, int x1, int y1, int x2, int y2);

	static void DrawRectangle(CDC* pDC, const CRect& rect, COLORREF clrPen, int nPenWidth = 1, int nPenStyle = PS_SOLID);
	static void DrawRectangle(CDC* pDC, CPen* pPen, const CRect& rect);
	static void DrawRectangle(CDC* pDC, const CRect& rect, COLORREF clrPen, int nPenWidth = 1, COLORREF clrBK = RGB(0, 0, 0), int nPenStyle = PS_SOLID);

	static void DrawRectText(CDC* pDC,CString strText,RECT rect,CFont* pFont,COLORREF clrText,UINT uFormat,E_TEXT_LAYOUT eLayout = E_TLAYOUT_HOR);		// 在矩形区域内显示文字

	static void FillPolygon(CDC* pDC,CPoint* pPoint,int nCount,COLORREF clr,BYTE byAlpha);
	static void DrawCurve(CDC* pDC,CPoint * pPoint,int nCount, COLORREF clr,int nWidth = 1,BYTE byAlpha = 0XFF);
private:
	GDIOBJMAP	m_mapGDIObj;
};
 
GW_EXT_CLASS GDILib* GetGDILib();

#ifdef SUPPORT_GDIPLUS
 
#define  GWBitmap GWBitmapEx

class GW_EXT_CLASS GWBitmapEx : public SafeThreadBase<GWBitmapEx>
{
public:
	GWBitmapEx(LPCTSTR lpFileName = NULL);
	~GWBitmapEx();

	BOOL LoadFromFile(LPCTSTR lpFileName);
	BOOL LoadFromRes(LPCTSTR lpResource)
	{
		return LoadFromFile(lpResource);
	}
	BOOL LoadFromRes(UINT uResID);

	int  Width();
	int  Height();
	CSize Size();

	void Draw(CDC* pDC);
	void Draw(CDC* pDC, const CRect& rect);
	void Draw(CDC* pDC,  CRect& rect, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight);
	void WrapModeTileDraw(CDC* pDC, CRect& rect);
	void TransparentDraw(CDC* pDC, const CRect& rect, COLORREF crTransparent);
	void TransparentDraw(CDC* pDC, const CRect& rect, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, COLORREF crTransparent);
		
	void Destory();

	void Play(HWND hWnd, BOOL bLoop = TRUE);
	void Stop();

	inline BOOL IsMultiFrames() const { return m_nFrameCount > 1; }
	inline UINT GetActiveFrame() const { return m_nActiveFrame; }
	inline UINT GetFrameCount() const { return m_nFrameCount; }
	inline void SetSleepTime(UINT nSleepTime) { m_nSleepTime = nSleepTime; }

	void SetActiveFrame(UINT nFrame);
	void Move2Rect(const CRect& rect);
private:
	BOOL InitImageFrameInfo();

	UINT PlayThread();

	HBITMAP GetHBitmapObj(COLORREF crTransparent);

	GWBitmapEx(const GWBitmapEx& bmp);
	GWBitmapEx& operator =(const GWBitmapEx& bmp);
private:
	Gdiplus::Bitmap	*m_pBmp;

	UINT m_nFrameCount;
	UINT m_nActiveFrame;

	typedef vector<long> INTVEC;
	INTVEC m_vecFrameDelay;

	UINT m_nSleepTime;
	BOOL m_bLoop;
	CRect m_rcBmp;
	volatile BOOL m_bWorking;

	const GUID  c_GifGuid;

	HWND	m_hAttachWnd;
};
#else
class GW_EXT_CLASS GWBitmap : public CBitmap
{
public:
	BOOL LoadFromRes(UINT uID)
	{
		Destroy();

		return LoadBitmap(uID);
	}

	BOOL LoadFromRes(LPCTSTR lpResource)
	{
		return LoadFromFile(lpResource);
	}

	BOOL LoadFromFile(LPCTSTR lpFileName);
	
	int  Width()
	{
		return Size().cx;
	}

	int  Height()
	{
		return Size().cy;
	}

	CSize Size();

	void TransparentBlt(CDC *pDC, CRect& rc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, COLORREF clrMask);

	void Draw(CDC* pDC, CRect& rect);
	void Draw(CDC* pDC, CRect& rect, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight);

	~GWBitmap()
	{
		Destroy();
	}

	void Destroy();
};

#endif