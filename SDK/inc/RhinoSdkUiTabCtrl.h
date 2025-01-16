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


// CRhinoUiTabCtrl

class RHINO_SDK_CLASS CRhinoUiTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CRhinoUiTabCtrl)

public:
	CRhinoUiTabCtrl();
	virtual ~CRhinoUiTabCtrl();

public:

protected:

protected:
	DECLARE_MESSAGE_MAP()

public:
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
  virtual void PreSubclassWindow();
public:
  virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
  afx_msg void OnNcPaint();
};


