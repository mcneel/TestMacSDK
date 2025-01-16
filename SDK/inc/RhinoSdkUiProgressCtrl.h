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


// CRhinoUiProgressCtrl

class RHINO_SDK_CLASS CRhinoUiProgressCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CRhinoUiProgressCtrl)

public:
	CRhinoUiProgressCtrl();
	virtual ~CRhinoUiProgressCtrl();

  enum display_mode
  {
    display_test = 1,
    display_percent = 2,
    display_text_and_percent = 3,
    // for internal use only, this allows adding additional values without
    // breaking the SDK in the future
    force_32bit_display_mode = 0xFFFFFFFF
  };

public:
  void SetRange(int nLower, int nUpper);
  // Sets the lower and upper range
  int SetStep(int nStep);
  // Sets the step increment

  int SetPos(int nPos);
  // Sets the position
  int StepIt();
  // Advances the current position by a step
  int OffsetPos(int nPos);
  // Advances the current position by a specified increment

  CString  GetText() {return m_strText;}
  void SetText(LPCTSTR lpText);
  // Gets and sets the text
  BOOL32 SetTextFont(const LOGFONT& lf);
  // Sets the text font
  void SetShowStyle(int nStyle);
  // Sets the text display style
  void SetShowText(bool bShow);
  // Sets the text visibility

  COLORREF GetBkColor() {return m_cfBackground;}
	void SetBkColor(COLORREF cf) {m_cfBackground = cf;}
  // Get an set the background color
	COLORREF GetForeColor() {return m_cfForeground;}
	void SetForeColor(COLORREF cf) {m_cfForeground = cf;}
  // Get an set the foreground color
	COLORREF GetTextBkColor() {return m_cfTextBackground;}
	void SetTextBkColor(COLORREF cf) {m_cfTextBackground = cf;}
  // Get an set the text background color
	COLORREF GetTextForeColor() {return m_cfTextForeground;}
	void SetTextForeColor(COLORREF cf) {m_cfTextForeground = cf;}

protected:
  int      m_nPos;             // Current position
	int      m_nStepSize;        // Step increment
  int      m_nMax;             // Maximum range value
  int      m_nMin;             // Minimum range value
  int      m_nBarWidth;        // Vidth of the progress bar
  int      m_nShowStyle;       // Text show style
  bool     m_bShowText;        // Text visibility
  CFont    m_Font;             // Text font
  CString  m_strText;          // Progress bar text
	COLORREF m_cfBackground;     // Background color
  COLORREF m_cfForeground;     // Foreground color
	COLORREF m_cfTextBackground; // Text background color
  COLORREF m_cfTextForeground; // Text foreground color

protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg BOOL32 OnEraseBkgnd(CDC* pDC);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg LRESULT OnSetText( WPARAM, LPARAM);
  afx_msg LRESULT OnGetText( WPARAM, LPARAM);
  afx_msg void OnPaint();
};
