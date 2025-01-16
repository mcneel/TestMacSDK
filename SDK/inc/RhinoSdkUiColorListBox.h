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

#include "RhinoSdkUiCheckListBox.h"

// CRhinoUiColorListBox

class RHINO_SDK_CLASS CRhinoUiColorListBox : public CRhinoUiListBox
{
	DECLARE_DYNAMIC(CRhinoUiColorListBox)

public:
	CRhinoUiColorListBox();
	virtual ~CRhinoUiColorListBox();

public:
  bool AddColorItem( const char* lpsColorName, COLORREF color);
  bool AddColorItem( const wchar_t* lpsColorName, COLORREF color);

  void SetColorPalette( HPALETTE pal);

  CRhinoUiColorComboBoxItem* ItemFromIndex( int nIndex);

protected:
  bool _AddColorItem( LPCTSTR lpsColorName, COLORREF color);

protected:
  CObArray m_items;
	HPALETTE m_hPalette;

protected:
	DECLARE_MESSAGE_MAP()

public:
  virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
  virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};
