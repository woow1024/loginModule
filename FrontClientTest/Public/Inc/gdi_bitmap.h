#pragma once

#include "autoobj.h"
class CBitmapEx : public CBitmap
{
public:
	BOOL LoadFromRes(UINT uID)
	{
		CAutoResource res;

		return LoadBitmap(uID);
	}

	BOOL LoadFromRes(LPCTSTR lpResource)
	{
		if(NULL == lpResource)
		{
			return FALSE;
		}
		if(this->GetSafeHandle())
		{
			this->DeleteObject();
		}
		HBITMAP hBMP = (HBITMAP)::LoadImage(NULL, lpResource, IMAGE_BITMAP,  0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR);
		//		ASSERT(hBMP != NULL);

		if (hBMP)
		{ 
			return Attach(hBMP);
		}

		return FALSE;
	}

	int  Width()
	{
		return Size().cx;
	}

	int  Height()
	{
		return Size().cy;
	}

	CSize Size()
	{
		BITMAP bm = {0};
		if (m_hObject != NULL)
		{
			GetBitmap(&bm);
		}

		return CSize(bm.bmWidth, bm.bmHeight);
	}

	void StretchDraw(CDC *pDC, CRect& rc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight)
	{
		CDC mDC;
		mDC.CreateCompatibleDC(pDC);
		CBitmap * pBmp = mDC.SelectObject(this);

		pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mDC, xSrc, ySrc, nSrcWidth, nSrcHeight, SRCCOPY);

		mDC.SelectObject(pBmp);
		mDC.DeleteDC();
	}
	void TransparentBlt(CDC *pDC, CRect& rc, COLORREF clrMask = RGB(255, 0, 255))
	{
		int nBmpWidth = Width();
		int nBmpHeight = Height();
		TransparentBlt(pDC, rc, 0, 0, nBmpWidth, nBmpHeight, clrMask);
	}
	void TransparentBlt(CDC *pDC, CRect& rc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, COLORREF clrMask)
	{
		CDC mDC;
		mDC.CreateCompatibleDC(pDC);
		CBitmap * pBmp = mDC.SelectObject(this);

		//pDC->TransparentBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mDC, xSrc, ySrc, nSrcWidth, nSrcHeight, clrMask);
		::TransparentBlt(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height(), mDC.GetSafeHdc(), xSrc, ySrc, nSrcWidth, nSrcHeight, clrMask);

		mDC.SelectObject(pBmp);
		mDC.DeleteDC();
	}

	void Draw(CDC* pDC, CRect& rc, BOOL bStretch = FALSE)
	{
		CDC mDC;
		mDC.CreateCompatibleDC(pDC);
		CBitmap * pBmp = mDC.SelectObject(this);

		BITMAP bm;
		GetBitmap(&bm);

		pDC-> SetStretchBltMode(HALFTONE); 

		if (bStretch)
		{
			pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		}
		else
		{
			pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mDC, 0, 0, SRCCOPY);
		}

		mDC.SelectObject(pBmp);
		mDC.DeleteDC();
	}

	~CBitmapEx()
	{
		if (GetSafeHandle() != NULL)
		{
			DeleteObject();
		}
	}
};







