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


// CRhinoUiStatic

class RHINO_SDK_CLASS  CRhinoUiStatic : public CStatic
{
	DECLARE_DYNAMIC(CRhinoUiStatic)

public:
	CRhinoUiStatic();
	virtual ~CRhinoUiStatic();

  // Returns the static window text
  CString GetText();

  // Sets the static window text
  void SetText( const char* lpsText );
  void SetText( const wchar_t* lpsText );

  bool TextIsFilePath();
  void SetTextIsFilePath( bool);

protected:
  bool m_bIsFilePath;

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
};


/*
Description:
  Owner drawn, drop-in replacement for CStatic that supports Rhino's color theme drawing.
  In most cases, you can simply replace CStatic with CRhinoUiStaticEx in your dialog box
  declaration.
*/
class RHINO_SDK_CLASS CRhinoUiStaticEx : public CStatic
{
  DECLARE_DYNAMIC(CRhinoUiStaticEx)

public:
  CRhinoUiStaticEx();
  virtual ~CRhinoUiStaticEx();

  // Gets and sets the static control text.
  // Setting will invalidate the control.
  CString GetText() const;
  void SetText(const wchar_t* pszText);

  // Sets a file path as the static control text.
  // File paths are drawn with DT_PATH_ELLIPSIS.
  // Setting will invalidate the control.
  bool IsFilePath() const;
  void SetFilePath(const wchar_t* pszPath);

  // Gets and sets the static control's text color.
  // This overrides Rhino's color theme drawing.
  // Setting will invalidate the control.
  COLORREF TextColor() const;
  void SetTextColor(COLORREF color);

  // Gets and sets the static control's text background color.
  // This overrides Rhino's color theme drawing.
  // Setting will invalidate the control.
  COLORREF BackgroundColor() const;
  void SetBackgroundColor(COLORREF color);

  // Another version of SetBackgroundColor
  void SetColor(COLORREF color);

  // Gets or sets the static control's text alignment.
  // Valid values are SS_LEFT, SS_CENTER, and SS_RIGHT.
  // Setting will invalidate the control.
  UINT GetAlignment() const;
  void SetAlignment(UINT alignment);

protected:
  DECLARE_MESSAGE_MAP()
  virtual void PreSubclassWindow();
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
  void SafeInvalidate();

private:
  UINT m_alignment = 0;
  bool m_file_path = false;
  COLORREF m_text_color = ON_UNSET_COLOR;
  COLORREF m_background_color = ON_UNSET_COLOR;
};
