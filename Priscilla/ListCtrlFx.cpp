﻿/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#include "../stdafx.h"
#include "ListCtrlFx.h"

IMPLEMENT_DYNAMIC(CListCtrlFx, CListCtrl)

CListCtrlFx::CListCtrlFx()
{
	m_bHighContrast = FALSE;
	m_RenderMode = SystemDraw;

	m_TextColor1 = RGB(0, 0, 0);
	m_TextColor2 = RGB(0, 0, 0);
	m_BkColor1   = RGB(255, 255, 255);
	m_BkColor2   = RGB(248, 248, 248);
	m_LineColor  = RGB(224, 224, 224);

	// Glass
	m_GlassColor = RGB(255, 255, 255);
	m_GlassColor = RGB(0, 0, 0);
	m_GlassAlpha = 128;
}

CListCtrlFx::~CListCtrlFx()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CListCtrlFx, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlFx::OnCustomdraw)
END_MESSAGE_MAP()

BOOL CListCtrlFx::InitControl(int x, int y, int width, int height, double zoomRatio, CDC* bgDC, int renderMode)
{
	m_X = (int)(x * zoomRatio);
	m_Y = (int)(y * zoomRatio);
	m_CtrlSize.cx = (int)(width * zoomRatio);
	m_CtrlSize.cy = (int)(height * zoomRatio);
	MoveWindow(m_X, m_Y, m_CtrlSize.cx, m_CtrlSize.cy);

	m_BgDC = bgDC;
	m_RenderMode = renderMode;

	if (renderMode & HighContrast)
	{
		m_bHighContrast = TRUE;
	}
	else if (renderMode & OwnerDrawGlass)
	{
		m_BgBitmap.DeleteObject();
		m_BgBitmap.CreateCompatibleBitmap(m_BgDC, m_CtrlSize.cx, m_CtrlSize.cy);
		CDC BgDC;
		BgDC.CreateCompatibleDC(m_BgDC);
		BgDC.SelectObject(m_BgBitmap);
		BgDC.BitBlt(0, 0, m_CtrlSize.cx, m_CtrlSize.cy, m_BgDC, m_X + 2, m_Y + 2, SRCCOPY);

		m_CtrlImage.Destroy();
		m_CtrlImage.Create(m_CtrlSize.cx, m_CtrlSize.cy, 32);

		RECT rect;
		rect.top = 0;
		rect.left = 0;
		rect.right = m_CtrlSize.cx;
		rect.bottom = m_CtrlSize.cy;

		m_CtrlBitmap.Detach();
		m_CtrlBitmap.Attach((HBITMAP)m_CtrlImage);

		DWORD length = m_CtrlSize.cx * m_CtrlSize.cy * 4;
		BYTE* bitmapBits = new BYTE[length];
		m_CtrlBitmap.GetBitmapBits(length, bitmapBits);

		BYTE r = (BYTE)GetRValue(m_GlassColor);
		BYTE g = (BYTE)GetGValue(m_GlassColor);
		BYTE b = (BYTE)GetBValue(m_GlassColor);
		BYTE a = m_GlassAlpha;

		for (int y = 0; y < m_CtrlSize.cy; y++)
		{
			for (int x = 0; x < m_CtrlSize.cx; x++)
			{
				bitmapBits[(y * m_CtrlSize.cx + x) * 4 + 0] = b;
				bitmapBits[(y * m_CtrlSize.cx + x) * 4 + 1] = g;
				bitmapBits[(y * m_CtrlSize.cx + x) * 4 + 2] = r;
				bitmapBits[(y * m_CtrlSize.cx + x) * 4 + 3] = a;
			}
		}

		m_CtrlBitmap.SetBitmapBits(length, bitmapBits);
		delete[] bitmapBits;

		SetupControlImage(m_BgBitmap, m_CtrlBitmap);

		SetBkImage((HBITMAP)m_CtrlBitmap);

		m_Header.InitControl(x, y, zoomRatio, bgDC, &m_CtrlBitmap, m_RenderMode);
	}
	else
	{
		m_Header.InitControl(x, y, zoomRatio, bgDC, NULL, m_RenderMode);
	}

	return TRUE;
}

void CListCtrlFx::SetupControlImage(CBitmap& bgBitmap, CBitmap& ctrlBitmap)
{
	if (m_BgDC->GetDeviceCaps(BITSPIXEL) * m_BgDC->GetDeviceCaps(PLANES) >= 24)
	{
		BITMAP CtlBmpInfo, DstBmpInfo;
		bgBitmap.GetBitmap(&DstBmpInfo);
		DWORD DstLineBytes = DstBmpInfo.bmWidthBytes;
		DWORD DstMemSize = DstLineBytes * DstBmpInfo.bmHeight;
		ctrlBitmap.GetBitmap(&CtlBmpInfo);
		DWORD CtlLineBytes = CtlBmpInfo.bmWidthBytes;
		DWORD CtlMemSize = CtlLineBytes * CtlBmpInfo.bmHeight;

		if (DstBmpInfo.bmWidthBytes != CtlBmpInfo.bmWidthBytes
			|| DstBmpInfo.bmHeight != CtlBmpInfo.bmHeight) {
			return;
		}

		BYTE* DstBuffer = new BYTE[DstMemSize];
		bgBitmap.GetBitmapBits(DstMemSize, DstBuffer);
		BYTE* CtlBuffer = new BYTE[CtlMemSize];
		ctrlBitmap.GetBitmapBits(CtlMemSize, CtlBuffer);

		int baseY = 0;
		for (LONG py = 0; py < DstBmpInfo.bmHeight; py++)
		{
			int dn = py * DstLineBytes;
			int cn = (baseY + py) * CtlLineBytes;
			for (LONG px = 0; px < DstBmpInfo.bmWidth; px++)
			{
				BYTE a = CtlBuffer[cn + 3];
				BYTE na = 255 - a;
				CtlBuffer[dn + 0] = (BYTE)((CtlBuffer[cn + 0] * a + DstBuffer[dn + 0] * na) / 255);
				CtlBuffer[dn + 1] = (BYTE)((CtlBuffer[cn + 1] * a + DstBuffer[dn + 1] * na) / 255);
				CtlBuffer[dn + 2] = (BYTE)((CtlBuffer[cn + 2] * a + DstBuffer[dn + 2] * na) / 255);
				dn += (DstBmpInfo.bmBitsPixel / 8);
				cn += (CtlBmpInfo.bmBitsPixel / 8);
			}
		}

		ctrlBitmap.SetBitmapBits(CtlMemSize, CtlBuffer);

		delete[] DstBuffer;
		delete[] CtlBuffer;
	}
}

void CListCtrlFx::OnCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_bHighContrast)
	{
		return;
	}

	LPNMLVCUSTOMDRAW lpLVCustomDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch(lpLVCustomDraw->nmcd.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		if(lpLVCustomDraw->nmcd.dwItemSpec % 2 == 0)
		{
			lpLVCustomDraw->clrText = m_TextColor1;
			lpLVCustomDraw->clrTextBk = m_BkColor1;
		}
		else
		{
			lpLVCustomDraw->clrText = m_TextColor2;
			lpLVCustomDraw->clrTextBk = m_BkColor2;
		}
		break;
	case CDDS_ITEMPOSTPAINT | CDDS_SUBITEM:
		{
			RECT rc;
			CBrush brush(m_LineColor);

			CHeaderCtrl* header = this->GetHeaderCtrl();
			if(header != NULL)
			{
				int count = header->GetItemCount();
				for(int i = 0; i < count; i++)
				{
					ListView_GetSubItemRect(m_hWnd, lpLVCustomDraw->nmcd.dwItemSpec, i, LVIR_LABEL, &rc);
					rc.left = rc.right - 1;
					FillRect(lpLVCustomDraw->nmcd.hdc, &rc, (HBRUSH) brush.GetSafeHandle());
				}
			}
		}
		break;
    default:
		break;
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;
	*pResult |= CDRF_NOTIFYITEMDRAW;
	*pResult |= CDRF_NOTIFYSUBITEMDRAW;
}

void CListCtrlFx::SetTextColor1(COLORREF color){m_TextColor1 = color;}
void CListCtrlFx::SetTextColor2(COLORREF color){m_TextColor2 = color;}
void CListCtrlFx::SetBkColor1(COLORREF color)  {m_BkColor1   = color;}
void CListCtrlFx::SetBkColor2(COLORREF color)  {m_BkColor2   = color;}
void CListCtrlFx::SetLineColor(COLORREF color) {m_LineColor  = color;}

COLORREF CListCtrlFx::GetTextColor1(){return m_TextColor1;}
COLORREF CListCtrlFx::GetTextColor2(){return m_TextColor2;}
COLORREF CListCtrlFx::GetBkColor1()  {return m_BkColor1;}
COLORREF CListCtrlFx::GetBkColor2()  {return m_BkColor2;}
COLORREF CListCtrlFx::GetLineColor() {return m_LineColor;}

void CListCtrlFx::SetFontEx(CString face, double zoomRatio, double fontRatio)
{
	LOGFONT logFont = {0};
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = (LONG)(-12 * zoomRatio * fontRatio);
	logFont.lfQuality = 6;
	if(face.GetLength() < 32)
	{
		wsprintf(logFont.lfFaceName, _T("%s"), face.GetString());
	}
	else
	{
		wsprintf(logFont.lfFaceName, _T(""));
	}

	m_Font.DeleteObject();
	m_Font.CreateFontIndirect(&logFont);
	SetFont(&m_Font);
}

void CListCtrlFx::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

//	CHeaderCtrlFx* pHeader = (CHeaderCtrlFx*)GetHeaderCtrl();
//	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}

void CListCtrlFx::EnableHeaderOwnerDraw(BOOL bOwnerDraw)
{
	if (m_RenderMode & HighContrast)
	{
		return;
	}
	else if (m_RenderMode & OwnerDrawGlass)
	{
		return;

		HDITEM hi;
		if (bOwnerDraw)
		{
			for (int i = 0; i < m_Header.GetItemCount(); i++)
			{
				m_Header.GetItem(i, &hi);
				hi.fmt |= HDF_OWNERDRAW;
				m_Header.SetItem(i, &hi);
			}
		}
		else
		{
			for (int i = 0; i < m_Header.GetItemCount(); i++)
			{
				m_Header.GetItem(i, &hi);
				hi.fmt &= ~HDF_OWNERDRAW;
				m_Header.SetItem(i, &hi);
			}
		}
	}
}