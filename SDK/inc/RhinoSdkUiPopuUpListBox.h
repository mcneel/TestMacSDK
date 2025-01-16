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


// CRhinoUiPopuUpListBox

class RHINO_SDK_CLASS CRhinoUiPopuUpListBox : public CListBox
{
	DECLARE_DYNAMIC(CRhinoUiPopuUpListBox)

public:
	CRhinoUiPopuUpListBox();
	virtual ~CRhinoUiPopuUpListBox();

public:
  void EnableSelectOnMouseMove( bool enable = true);
  bool SelectOnMouseMove() const;
  int LeftButtonDownOnX() const;
  int PageCount() const;
  int WidestString() const;
  int RowFromPoint( CPoint point, int& row) const;
  CFont m_font;

protected:
  class CRhinoUiPopUpListWnd* PopUpWindow();

protected:
  friend class CRhinoUiPopUpListWnd;

  bool  m_select_on_mouse_move;
  bool  m_adjust_width;
  int   m_lbutton_down_on_item;
  int   m_lbutton_donw_on_x;
  CRhinoUiPopUpListWnd* m_list_wnd;
  
protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL32 PreTranslateMessage(MSG* pMsg);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg BOOL32 OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
