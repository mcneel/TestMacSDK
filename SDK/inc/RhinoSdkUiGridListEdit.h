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


// CRhinoUiGridListEdit

class RHINO_SDK_CLASS CRhinoUiGridListEdit : public CEdit
{
	DECLARE_DYNAMIC(CRhinoUiGridListEdit)

public:
	CRhinoUiGridListEdit();
	virtual ~CRhinoUiGridListEdit();

protected:
	class CRhinoUiGridListCtrl* GetGridListCtrl();

public:
	int m_x;
	int m_y;
  int m_cx;
  int m_cy;
protected:
	UINT m_nChar;
  UINT m_nRepCnt;
  UINT m_nFlags;

protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};


