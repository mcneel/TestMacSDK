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


// CRhinoUiColorButton
//
// This class is provided strictly for backwards compatibility
//
class RHINO_SDK_CLASS CRhinoUiColorButton : public CRhinoUiButton
{
	DECLARE_DYNAMIC(CRhinoUiColorButton)

public:
	CRhinoUiColorButton();
	virtual ~CRhinoUiColorButton();

public:
  void Initialize( COLORREF color = ::GetSysColor( COLOR_BTNFACE ), HPALETTE hPalette = 0, bool bSuppressTextWhenEnabled = true);
	COLORREF Color();
	void SetColor( COLORREF cr);
	void SetColor( const unsigned char r, const unsigned char g, const unsigned char b );

	HPALETTE ColorPalette();
	void SetColorPalette( HPALETTE hp);

	virtual bool OnButtonClicked();

  // CRhinoUiButton override
  virtual bool OnColorButtonClicked();

  // Use ON_REGISTERED_MESSAGE( CRhinoUiColorButton::m_on_button_clicked_msg_id, OnRhinoUiColorButtonClicked)
  // macro to get button click notification.  The WPARAM and LPARAM are the same as BN_CLICKED notification.
  static const UINT m_on_button_clicked_msg_id;

protected:
	DECLARE_MESSAGE_MAP()
};
