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


// CRhinoUiDragBitmapWnd

class RHINO_SDK_CLASS CRhinoUiDragBitmapWnd : public CWnd
{
	DECLARE_DYNAMIC(CRhinoUiDragBitmapWnd)

public:
	CRhinoUiDragBitmapWnd();
	virtual ~CRhinoUiDragBitmapWnd();

  bool CreateBitmapDragWindow( const CRhinoDib& dib, CWnd* pWndParent = NULL);
  bool ShowDragWindow( bool bShow, LPPOINT pPtScreen);
  bool TrackDragWindow( CPoint ptScreen);
  bool SetDragBitmap( const CRhinoDib& dib);
  void SetBitapTransparentColor( COLORREF cr);
  void SetBasePoint( CPoint pt);
  void SetOffset( CSize szOffset);

  bool SetOffsetLeft( bool b);
  bool OffsetLeft() const;

  int m_dib_id;

protected:
  HWND m_hWndParent;
  CString m_sWndCls;
  CRhinoDib m_dib;
  bool m_bOffsetLeft;
  CSize m_szOffset;
  CPoint m_ptBase;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
};


