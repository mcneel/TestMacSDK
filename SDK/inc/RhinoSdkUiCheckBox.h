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


// CRhinoUiCheckBox
class RHINO_SDK_CLASS CRhinoUiCheckBox : public CButton
{
	DECLARE_DYNAMIC(CRhinoUiCheckBox)

public:
	CRhinoUiCheckBox();
	virtual ~CRhinoUiCheckBox();

  // Defaults to on
  bool ToggleStateOnRightClick() const;
  bool SetToggleStateOnRightClick( bool b);

protected:
  bool m_bRButtonDown;
  bool m_bAllowRightButtonClick;

protected:
	DECLARE_MESSAGE_MAP()
public:
//  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


