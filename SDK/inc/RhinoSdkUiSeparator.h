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


// CRhinoUiSeparator

/*
Description
  CRhinoUiSeparator is an enhancement to the MFC CRhinoUiStatic that, in addition
  to displaying text, draws a separator line. This class as an alternative
  to using two statistics when displaying text w/ separator lines in options
  and properties dialog boxes and property pages.

Remarks:
  When laying out horizontal separators, place your static controls as usual,
  but make them as wide as you want the horizontal line to draw. You can also
  set the alignment of the text (left, center, or right), and set the No
  Prefix style. Those are the only static control styles currently supported.

  Each static control that will be used as a separator should have a real,
  unique ID (not IDC_STATIC). Using ClassWizard, create member variables of
  type CRhinoUiStatic for each separator, and then go to the ClassWizard-generated
  code and change the type of the variables to CRhinoUiSeparator.
*/

class RHINO_SDK_CLASS CRhinoUiSeparator : public CRhinoUiStatic
{
	DECLARE_DYNAMIC(CRhinoUiSeparator)

public:
	CRhinoUiSeparator();
	virtual ~CRhinoUiSeparator();

protected:
	DECLARE_MESSAGE_MAP()
  afx_msg void OnEnable(BOOL32 bEnable);
	afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
