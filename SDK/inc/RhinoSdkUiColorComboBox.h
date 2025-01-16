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


/////////////////////////////////////////////////////////////////////////////
// CRhinoUiColorComboBoxItem

class RHINO_SDK_CLASS CRhinoUiColorComboBoxItem : public CObject
{
public:
  CRhinoUiColorComboBoxItem();
  CRhinoUiColorComboBoxItem( LPCTSTR lpsColor, COLORREF color);
  CRhinoUiColorComboBoxItem( const CRhinoUiColorComboBoxItem& );

  CRhinoUiColorComboBoxItem &operator=( const CRhinoUiColorComboBoxItem& );

  CString  m_string;
  COLORREF m_color;
};


/////////////////////////////////////////////////////////////////////////////
// CRhinoUiColorComboBox

class RHINO_SDK_CLASS CRhinoUiColorComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CRhinoUiColorComboBox)

public:
	CRhinoUiColorComboBox();
	virtual ~CRhinoUiColorComboBox();

	bool AddColorItem( const wchar_t* lpsColorName, COLORREF color);
	bool AddColorItem( const char* lpsColorName, COLORREF color);

  void SetColorPalette( HPALETTE pal);

  CRhinoUiColorComboBoxItem* ItemFromIndex( int nIndex);

protected:
	bool _AddColorItem( LPCTSTR lpsColorName, COLORREF color);

public:
  CObArray m_items;
protected:
	HPALETTE m_hPalette;

protected:
	DECLARE_MESSAGE_MAP()
  virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
  virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
  virtual void PreSubclassWindow();
};
