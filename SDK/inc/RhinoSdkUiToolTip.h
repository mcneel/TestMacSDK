/* $NoKeywords: $ */
/*
//
// Copyright (c) 1993-2007 Robert McNeel & Associates. All rights reserved.
// Rhinoceros is a registered trademark of Robert McNeel & Associates.
//
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
// ALL IMPLIED WARRANTIES OF FITNESS FOR ANY PARTICULAR PURPOSE AND OF
// MERCHANTABILITY ARE HEREBY DISCLAIMED.
//				
// For complete Rhino SDK copyright information see <http://www.rhino3d.com/developer>.
//
////////////////////////////////////////////////////////////////
*/

#pragma once


// CRhinoUiToolTip

class RHINO_SDK_CLASS CRhinoUiToolTip : public CWnd
{
	DECLARE_DYNAMIC(CRhinoUiToolTip)
public:
	CRhinoUiToolTip();
	virtual ~CRhinoUiToolTip();

  virtual bool CreateToolTip( CWnd* pWndParent);
  virtual bool ShowToolTip( bool bShow, LPRECT lpRect);

  void DestroyToolTip();

  const wchar_t* ToolTipText() const;
  bool SetToolTipText( const wchar_t* lps);

  // If CRhinoUiPaintManager::unset_color is returned then GetSysColor( COLOR_INFOTEXT)
  // is used
  COLORREF ToolTipTextColor() const;
  // If CRhinoUiPaintManager::unset_color passed for color value then
  // GetSysColor( COLOR_INFOTEXT) is used
  void SetToolTipTextColor( COLORREF cr);
  // If CRhinoUiPaintManager::unset_color is returned then GetSysColor( COLOR_INFOBK)
  // is used
  COLORREF ToolTipBackgroundColor() const;
  // If CRhinoUiPaintManager::unset_color passed for color value then
  // GetSysColor( COLOR_INFOBK) is used
  void SetToolTipBackgroundColor( COLORREF cr);
  // If true then the tool tip will hide itself on a WM_MOUSEMOVE event
  bool HideOnMouseOver() const;
  bool SetHideOnMouseOver( bool b);
protected:
  virtual bool RecalcLayout( CSize& szWindow);

  COLORREF DrawTextColor() const;
  COLORREF DrawBkgrndColor() const;

  void PaintBkgrnd( CDC& dc);

protected:
  CString m_sWndCls;
  ON_wString m_wTipText;
  COLORREF m_crText;
  COLORREF m_crBkgrnd;
  HWND m_hWndParent;
  int m_cx;
  int m_cy;
  CSize m_szBorders;
  CRect m_rTip;
  bool m_bRecalcLayout;
  bool m_bDestroyTT;
  bool m_bHideOnMouseOver;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
  afx_msg void OnDestroy();
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
  afx_msg void OnActivateApp(BOOL32 bActive, DWORD dwThreadID);
};


