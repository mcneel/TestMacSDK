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

//
// Ported from CRhinoUiBitmapButton
//
// Usage -- a replacement for the MFC CBitmapButton
// 1. include an owner-draw button in your dialog
// 2. declare a CRhinoUiBitmapButton member in the CDialog code
// 3. hook in the CRhinoUiBitmapButton using a call to AutoLoad
//
// the bitmap resource specified in AutoLoad must be divisible into
// 4 equally sized images that represent (left to right) the
// up, down, focused and disabled states of the button

// CRhinoUiBitmapButton
class RHINO_SDK_CLASS CRhinoUiBitmapButton : public CButton
{
	DECLARE_DYNAMIC(CRhinoUiBitmapButton)

public:
	CRhinoUiBitmapButton();
	virtual ~CRhinoUiBitmapButton();

public:
	bool LoadBitmap( UINT idBitmapResource, AFX_MODULE_STATE* pModuleState = NULL);
	void SizeToContent();
	bool AutoLoad(UINT nID, CWnd* pParent, UINT idBitmapResource);

protected:
	CImageList m_image;
	UINT m_idResource;

protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

  afx_msg void OnSysColorChange();
};


