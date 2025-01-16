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


// CRhinoUiDockBarClientWnd

class RHINO_SDK_CLASS CRhinoUiDockBarClientWnd : public CWnd
{
	DECLARE_DYNAMIC(CRhinoUiDockBarClientWnd)

public:
  //   
  // Description:
  //   Constructor
  //
  // Parameters:
  //   pCB [in] Control bar that owns this window
  //
  // See Also:
  //   CRhinoUiDockBarClientWnd::RhinoControlBar()
  //   
	CRhinoUiDockBarClientWnd( CRhinoUiDockBar* pCB);
  //   
  // Description:
  //   Destructor
  //
	virtual ~CRhinoUiDockBarClientWnd();
  //   
  // Description:
  //   Rhino control bar this window is embedded in
  //
  // Returns:
  //   Rhino control bar this window is embedded in
  //
  CRhinoUiDockBar* GetRhinoDockBar();
private:
  CRhinoUiDockBar* m_pCB;

protected:
	DECLARE_MESSAGE_MAP()

protected:
  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


