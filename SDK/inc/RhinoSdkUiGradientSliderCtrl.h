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


// CRhinoUiGradientSliderCtrl

class RHINO_SDK_CLASS CRhinoUiGradientSliderCtrl : public CRhinoUiSliderCtrl
{
	DECLARE_DYNAMIC(CRhinoUiGradientSliderCtrl)

public:
	CRhinoUiGradientSliderCtrl();
	virtual ~CRhinoUiGradientSliderCtrl();

public:
  void InitializeGradientSlider( COLORREF from, COLORREF to, HPALETTE hPalette = NULL);

  HPALETTE ColorPalette();
	void     SetColorPalette( HPALETTE hPalette);

  COLORREF FromColor();
  void     SetFromColor( COLORREF color);
	void     SetFromColor( const unsigned char r, const unsigned char g, const unsigned char b );

  COLORREF ToColor();
  void     SetToColor( COLORREF color);
	void     SetToColor( const unsigned char r, const unsigned char g, const unsigned char b );

private:
  COLORREF m_FromColor;
  COLORREF m_ToColor;
	HPALETTE m_hPalette;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
};


