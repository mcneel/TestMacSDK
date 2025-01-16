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


// CRhinoUiHyperlink

class RHINO_SDK_CLASS CRhinoUiHyperlink : public CStatic
{
	DECLARE_DYNAMIC(CRhinoUiHyperlink)

public:
	CRhinoUiHyperlink( LPCTSTR lpLink = NULL, bool bDeleteOnDestroy = false);

  virtual ~CRhinoUiHyperlink();

public:
  CString m_strLink;
	COLORREF m_cfCurrentColor;
  COLORREF m_cfUnvisitedColor;
	COLORREF m_cfVisitedColor;
	HCURSOR m_hHandCursor;

protected:
	CFont m_Font;
	BOOL32 m_bDeleteOnDestroy;

protected:
	DECLARE_MESSAGE_MAP()
  virtual void PostNcDestroy();
public:
  afx_msg LRESULT OnNcHitTest(CPoint point);
  afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg BOOL32 OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


