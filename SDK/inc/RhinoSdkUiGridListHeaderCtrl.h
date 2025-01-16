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


// CRhinoUiGridListHeaderCtrl

class RHINO_SDK_CLASS CRhinoUiGridListHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CRhinoUiGridListHeaderCtrl)

public:
	CRhinoUiGridListHeaderCtrl();
	virtual ~CRhinoUiGridListHeaderCtrl();

public:
  void AllowColumnResizing( bool b);
  bool AllowColumnResizing();
  
  void DisableColumResizing( int nInColumn);
  bool ColumnResizable( int nColumn);
  
  int CursorColumnFromPoint( CPoint point);
  int ColumnFromPoint( CPoint point);

  CListCtrl *ListCtrl();

  LPARAM GetColumnLParam( int nCol);

protected:
  bool       m_bAllowColumnResizing;
  int        m_nCursorColumn;
  CUIntArray m_DisableColumnSizingList;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg LRESULT OnNcHitTest(CPoint point);
  afx_msg BOOL32 OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


