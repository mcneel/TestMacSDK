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


// CRhinoUiFontComboBox


class RHINO_SDK_CLASS CRhinoUiFontComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CRhinoUiFontComboBox)

public:
  CRhinoUiFontComboBox();
	virtual ~CRhinoUiFontComboBox();

public:
  void SetHilightColors( COLORREF hilight, COLORREF hilightText);
  void SetNormalColors( COLORREF clrBkgnd, COLORREF clrText);
  
  void FillFonts( char* focus);
  void FillFonts( wchar_t* focus = NULL);
  
  bool Varies();
  void SetVaries( bool b);

#if defined (ON_RUNTIME_APPLE)
  bool SetSelFont (const ON_Font&);
#endif

  int GetSelFont( LOGFONT& );
  int GetFontListIndex( const char* lpsFontFaceName);
  int GetFontListIndex( const wchar_t* lpsFontFaceName);

  bool OnlyShowTrueTypeFonts() const;
  bool SetOnlyShowTrueTypeFonts( bool b);

  bool IgnoreZeroTypeFonts() const;
  bool SetIgnoreZeroTypeFonts(bool value);

  LOGFONT* GetLogfontFromListIndex( int index);

protected:
  int _GetFontListIndex( LPCTSTR lpsFontFaceName);
  void DrawDefault( LPDRAWITEMSTRUCT );
  void DrawFont( LPDRAWITEMSTRUCT );
  void InitFonts( );     

public:
  CPtrArray m_font_list;
protected:
  COLORREF  m_clrHilight; 
  COLORREF  m_clrNormalText;
  COLORREF  m_clrHilightText; 
  COLORREF  m_clrBkgnd;  
  bool      m_bInitOver;
  bool      m_bTrueTypeOnly;
  bool      m_bVaries;// Added varies support 8/2/07 LW

protected:
	DECLARE_MESSAGE_MAP()
public:
  virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
  virtual int CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
  virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
#if defined (ON_OS_WINDOWS)
  virtual void PreSubclassWindow();
#endif

public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
};


